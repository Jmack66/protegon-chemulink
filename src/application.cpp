#include "plot_scene.h"
#include "protegon/protegon.h"
#include "vfx/light.h"

using namespace ptgn;

constexpr const V2_int resolution{ 1280, 720 };

static void TransitionScene(
	std::string_view from, std::string_view to, milliseconds duration = milliseconds{ 250 }
) {
	game.scene.TransitionActive(from, to, { TransitionType::Fade, milliseconds{ 1000 } });
}

class BlockMenu : public Scene {
public:
	static const size_t num_buttons = 16;
	// Button b_mixer;
	// Button b_heater;
	// Button b_cooler;
	// Button b_disolver;
	// Button b_filter;
	// Button b_evaporator;
	// Button b_crystaliser;
	// Button b_distiller;
	// Button b_titrater;
	// Button b_seperator;
	// Button b_observer;
	// Button b_b1;
	// Button b_b2;
	// Button b_b3;
	// Button b_b4;
	// Button b_b5;
	// Text mixer{"Mixer", color::Gray};
	// Text heater{"Heater", color::Gray};
	// Text cooler{"Cooler", color::Gray};
	// Text disolver{"Disolver", color::Gray};
	// Text filter{"Filter", color::Gray};
	// Text evaporator{"Evaporator", color::Gray};
	// Text crystaliser{"Crystaliser", color::Gray};
	// Text distiller{"Distiller", color::Gray};
	// Text titrater{"Titrater", color::Gray};
	// Text seperator{"Seperator", color::Gray};
	// Text observer{"Observer", color::Gray};

	// std::array<Text, num_buttons> button_names {mixer, heater, cooler, disolver, filter,
	// evaporator, crystaliser, distiller, titrater, seperator, observer, b1, b2, b3, b4, b5};
	// std::array<Button, num_buttons> button_group {b_mixer, b_heater, b_cooler, b_disolver,
	// b_filter, b_evaporator, b_crystaliser, b_distiller, b_titrater, b_seperator, b_observer,
	// b_b1, b_b2, b_b3, b_b4, b_b5}; std::array<std::string_view, num_buttons> button_names
	// {"Mixer", "Heater", "Cooler", "Disolver", "Filter", "Evaporator", "Crystaliser", "Distiller",
	// "Titrater", "Sperator", "Observer", "Blank1", "Blank2", "Blank3", "Blank4", "Blank5"};
	std::vector<Button> buttons;

	BlockMenu() {}

	Button CreateButton(const std::string_view& name) {
		Button b;
		b.Set<ButtonProperty::Toggleable>(true);
		b.Set<ButtonProperty::BackgroundColor>(color::White);
		b.Set<ButtonProperty::BackgroundColor>(color::DarkBlue, ButtonState::Hover);
		b.Set<ButtonProperty::Text>(Text{ name, color::Gray });
		b.Set<ButtonProperty::OnActivate>([=]() { PTGN_LOG("Pressed button: ", name); });
		return b;
	}

	// Button CreateButton(const V2_float& center, const Text name) {
	// Button b;
	// b.SetRect(Rect{ center, { 100, 50 }, Origin::Center });
	// b.Set<ButtonProperty::Toggleable>(true);
	// b.Set<ButtonProperty::BackgroundColor>(color::White);
	// b.Set<ButtonProperty::BackgroundColor>(color::Red, ButtonState::Hover);
	// b.Set<ButtonProperty::Text>(name);
	// b.Set<ButtonProperty::OnActivate>([=]() {
	// PTGN_LOG("Pressed button at: ", center);
	//});
	// return b;
	//}

	void Init() override {
		buttons.emplace_back(CreateButton("Mixer"));
		buttons.emplace_back(CreateButton("Heater"));
		buttons.emplace_back(CreateButton("Cooler"));
		buttons.emplace_back(CreateButton("Dissolver"));
		buttons.emplace_back(CreateButton("Filter"));
		buttons.emplace_back(CreateButton("Evaporator"));
		buttons.emplace_back(CreateButton("Crystaliser"));
		buttons.emplace_back(CreateButton("Distiller"));
		buttons.emplace_back(CreateButton("Titrator"));
		buttons.emplace_back(CreateButton("Seperator"));
		buttons.emplace_back(CreateButton("Observer"));
		buttons.emplace_back(CreateButton("Observer1"));
		buttons.emplace_back(CreateButton("Observer2"));
		buttons.emplace_back(CreateButton("Observer3"));
		buttons.emplace_back(CreateButton("Observer4"));
		buttons.emplace_back(CreateButton("Observer5"));
		game.window.SetTitle("BlockMenu");
		game.window.SetSize(resolution);
		int x_pad = 250;
		int y_pad = 150;
		int idx	  = 0;
		for (int i = 0; i < num_buttons / 4; ++i) {
			for (int j = 0; j < num_buttons / 4; ++j) {
				buttons[idx].SetRect(Rect{
					{ 125.0f + x_pad * i, 125.0f + y_pad * j }, { 100, 50 }, Origin::Center });
				idx++;
			}
		}
	}

	void Update() final {
		if (game.input.KeyDown(Key::I)) {
			TransitionScene("BlockMenu", "PlacementMenu");
		}
		if (game.input.KeyDown(Key::P)) {
			game.scene.LoadActive<PlotScene>("PlotScene");
			TransitionScene("BlockMenu", "PlotScene");
		}

		for (Button b : buttons) {
			b.Draw();
		}
	}
};

class PlacementMenu : public Scene {
public:
	Button dragging;
	ToggleButtonGroup buttons;

	PlacementMenu() {}

	void Init() override {
		game.window.SetTitle("PlacementMenu");
		game.window.SetSize(resolution);
	}

	void Update() final {
		V2_int mouse_pos = game.input.GetMousePosition();
		mouse_pos.Draw(Color{ 0, 0, 255, 30 }, 2.0f);

		if (game.input.KeyDown(Key::B)) {
			auto id			 = buttons.Size() + 1;
			std::string id_s = std::to_string(id);
		}

		if (game.input.MouseDown(Mouse::Left)) {
			buttons.ForEachValue([=](Button& b) {
				auto rect = b.GetRect();
				if (rect.Overlaps(mouse_pos)) {
					dragging = b;
				}
			});
		} else if (game.input.MouseUp(Mouse::Left)) {
			dragging = {};
		}

		if (dragging != Button{}) {
			auto rect	  = dragging.GetRect();
			rect.position = mouse_pos;
			dragging.SetRect(rect);
		}
		Line line1{ dragging.GetRect().position, mouse_pos };
		line1.Draw(color::Purple, 3.0f);
		// auto c = camera.GetPrimary();
		buttons.Draw();
	}
};

class Simulink : public Scene {
public:
	void Init() override {
		game.scene.Load<BlockMenu>("BlockMenu");
		game.scene.Load<PlacementMenu>("PlacementMenu");
		game.scene.AddActive("BlockMenu");
	}
};

int main(int c, char** v) {
	game.Init("Simulink", resolution, color::White);
	game.scene.LoadActive<Simulink>("Simulink");
	return 0;
}