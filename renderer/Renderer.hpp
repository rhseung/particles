#pragma once
#include "../physics/Solver.hpp"
#include "../engine/common/Constraints.hpp"
#include <functional>
#include <algorithm>

using Stringf = std::function<std::string()>;
using StringAndColorf = std::function<std::pair<std::string, sf::Color>()>;

class Renderer {
 private:
    sf::RenderTarget &target;
    sf::Font font;
    std::vector<std::tuple<StringAndColorf, Vec2, uint32_t>> texts;  // text&color, position, character_size

 public:
    explicit Renderer(sf::RenderTarget &target): target{target} {
        font.loadFromFile(R"(D:\code-hub\particles\resource\CascadiaCode-Regular.otf)");
    }

    void setFont(const std::string &path) {
        font.loadFromFile(path);
    }

    void addText(const StringAndColorf& text_function, const uint32_t character_size = 15) {
        texts.emplace_back(text_function, Vec2{10.f, 10.f + 25.f * (float)texts.size()}, character_size);
    }

    void addText(const Stringf& text_function, const sf::Color &color = sf::Color::White, const uint32_t character_size = 15) {
        StringAndColorf f = [&color, &text_function]() {
            return std::make_pair(text_function(), color);
        };
        texts.emplace_back(f, Vec2{10.f, 10.f + 25.f * (float)texts.size()}, character_size);
    }

    void addText(const Stringf& text_function, const sf::Vector2f &position, const sf::Color &color = sf::Color::White, const uint32_t character_size = 15) {
        StringAndColorf f = [&color, &text_function]() {
            return std::make_pair(text_function(), color);
        };
        texts.emplace_back(f, position, character_size);
    }

    void render(Solver solver) const {
        // Render constraints
        for (int i = 0; i < solver.getConstraintCount(); ++i) {
            for (auto drawable: solver.getConstraint(i)->render()) {
                target.draw(*drawable);
            }
        }

        // Render objects
        for (int i = 0; i < solver.getBodyCount(); ++i) {
            for (auto drawable: solver.getBody(i)->render()) {
                target.draw(*drawable);
            }
        }

        float contact_radius = 5.f;
        for (auto manifold: solver.getManifolds()) {
            if (manifold.contact_count == 0)
                continue;

            // contact point circle
            if (manifold.contact_count == 1) {
                auto *circle = new sf::CircleShape(contact_radius);
                circle->setOrigin(contact_radius, contact_radius);
                circle->setPosition(manifold.contact1);
                circle->setFillColor(sf::Color::Red);
                target.draw(*circle);
            }
            else if (manifold.contact_count == 2) {
                auto *circle1 = new sf::CircleShape(contact_radius);
                circle1->setOrigin(contact_radius, contact_radius);
                circle1->setPosition(manifold.contact1);
                circle1->setFillColor(sf::Color::Red);
                target.draw(*circle1);

                auto *circle2 = new sf::CircleShape(contact_radius);
                circle2->setOrigin(contact_radius, contact_radius);
                circle2->setPosition(manifold.contact2);
                circle2->setFillColor(sf::Color::Red);
                target.draw(*circle2);
            }
        }

        // Render texts
        for (const auto &[stringf, position, character_size] : texts) {
            auto [string, color] = stringf();
            sf::Text text(string, font, character_size);
            text.setFillColor(color);
            text.setPosition(position);
            target.draw(text);
        }
    }
};
