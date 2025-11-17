#ifndef POKEAPI_H
#define POKEAPI_H

#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <pplx/pplx.h>
#include <iostream>
#include <string>
#include <map>
#include <vector>

using namespace web;
using namespace web::http;
using namespace web::http::client;

class Pokeapi {
private:
    std::string api_base = "https://pokeapi.co/api/v2";
    http_client_config client_config;
    http_request create_request(const std::string& endpoint, const std::string& method, const std::string& data = "") {
        http_request request;
        
        if (method == "GET") {
            request.set_method(methods::GET);
        } else if (method == "POST") {
            request.set_method(methods::POST);
        } else if (method == "PUT") {
            request.set_method(methods::PUT);
        } else if (method == "DEL") {
            request.set_method(methods::DEL);
        }
        
        request.set_request_uri(utility::conversions::to_string_t(endpoint));
        
        // Set headers
        request.headers().add(U("Accept"), U("text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7"));
        request.headers().add(U("Host"), U("pokeapi.co"));
        request.headers().add(U("User-Agent"), U("Mozilla/5.0 (X11; Linux x86_64; rv:109.0) Gecko/20100101 Firefox/115.0"));
        
        if (!data.empty() && (method == "POST" || method == "PUT")) {
            request.set_body(data);
        }
        
        return request;
    }
    
    pplx::task<json::value> make_api_call(const std::string& endpoint, const std::string& method, const std::string& data = "") {
        http_client client(utility::conversions::to_string_t(api_base), client_config);
        auto request = create_request(endpoint, method, data);

        return client.request(request)
            .then([](http_response response) {
                if (response.status_code() == status_codes::OK) {
                    return response.extract_json();
                } else {
                    json::value error_obj;
                    error_obj[U("error")] = json::value::string(
                        U("HTTP Error: ") + utility::conversions::to_string_t(std::to_string(response.status_code())));
                    error_obj[U("success")] = json::value::boolean(false);
                    return pplx::task_from_result(error_obj);
                }
            })
            .then([](pplx::task<json::value> previousTask) {
                try {
                    return previousTask.get();
                } catch (const std::exception& e) {
                    json::value error_obj;
                    error_obj[U("error")] = json::value::string(
                        U("Exception: ") + utility::conversions::to_string_t(e.what()));
                    error_obj[U("success")] = json::value::boolean(false);
                    return error_obj;
                }
            });
    }

public:
    Pokeapi() {
        client_config.set_validate_certificates(false);
    }

    // Encounter Conditions
    pplx::task<json::value> get_encounter_condition_list(int limit = 20, int offset = 0) {
        std::string endpoint = "/encounter-condition?limit=" + std::to_string(limit) + 
                              "&offset=" + std::to_string(offset);
        return make_api_call(endpoint,"GET");
    }

    pplx::task<json::value> get_encounter_condition_by_id(int condition_id) {
        return make_api_call("/encounter-condition/" + std::to_string(condition_id),"GET");
    }

    pplx::task<json::value> get_encounter_condition_by_name(const std::string& condition_name) {
        return make_api_call("/encounter-condition/" + condition_name,"GET");
    }

    // Encounter Condition Values
    pplx::task<json::value> get_encounter_condition_value_list(int limit = 20, int offset = 0) {
        std::string endpoint = "/encounter-condition-value?limit=" + std::to_string(limit) + 
                              "&offset=" + std::to_string(offset);
        return make_api_call(endpoint,"GET");
    }

    pplx::task<json::value> get_encounter_condition_value_by_id(int value_id) {
        return make_api_call("/encounter-condition-value/" + std::to_string(value_id),"GET");
    }

    pplx::task<json::value> get_encounter_condition_value_by_name(const std::string& value_name) {
        return make_api_call("/encounter-condition-value/" + value_name,"GET");
    }

    // Evolution Chains
    pplx::task<json::value> get_evolution_chain_list(int limit = 20, int offset = 0) {
        std::string endpoint = "/evolution-chain?limit=" + std::to_string(limit) + 
                              "&offset=" + std::to_string(offset);
        return make_api_call(endpoint,"GET");
    }

    pplx::task<json::value> get_evolution_chain_by_id(int chain_id) {
        return make_api_call("/evolution-chain/" + std::to_string(chain_id),"GET");
    }

    // Evolution Triggers
    pplx::task<json::value> get_evolution_trigger_list(int limit = 20, int offset = 0) {
        std::string endpoint = "/evolution-trigger?limit=" + std::to_string(limit) + 
                              "&offset=" + std::to_string(offset);
        return make_api_call(endpoint,"GET");
    }

    pplx::task<json::value> get_evolution_trigger_by_id(int trigger_id) {
        return make_api_call("/evolution-trigger/" + std::to_string(trigger_id),"GET");
    }

    pplx::task<json::value> get_evolution_trigger_by_name(const std::string& trigger_name) {
        return make_api_call("/evolution-trigger/" + trigger_name,"GET");
    }

    // Generations
    pplx::task<json::value> get_generation_list(int limit = 20, int offset = 0) {
        std::string endpoint = "/generation?limit=" + std::to_string(limit) + 
                              "&offset=" + std::to_string(offset);
        return make_api_call(endpoint,"GET");
    }

    pplx::task<json::value> get_generation_by_id(int generation_id) {
        return make_api_call("/generation/" + std::to_string(generation_id),"GET");
    }

    pplx::task<json::value> get_generation_by_name(const std::string& generation_name) {
        return make_api_call("/generation/" + generation_name,"GET");
    }

    // Pokedexes
    pplx::task<json::value> get_pokedex_list(int limit = 20, int offset = 0) {
        std::string endpoint = "/pokedex?limit=" + std::to_string(limit) + 
                              "&offset=" + std::to_string(offset);
        return make_api_call(endpoint,"GET");
    }

    pplx::task<json::value> get_pokedex_by_id(int pokedex_id) {
        return make_api_call("/pokedex/" + std::to_string(pokedex_id),"GET");
    }

    pplx::task<json::value> get_pokedex_by_name(const std::string& pokedex_name) {
        return make_api_call("/pokedex/" + pokedex_name,"GET");
    }

    // Versions
    pplx::task<json::value> get_version_list(int limit = 20, int offset = 0) {
        std::string endpoint = "/version?limit=" + std::to_string(limit) + 
                              "&offset=" + std::to_string(offset);
        return make_api_call(endpoint,"GET");
    }

    pplx::task<json::value> get_version_by_id(int version_id) {
        return make_api_call("/version/" + std::to_string(version_id),"GET");
    }

    pplx::task<json::value> get_version_by_name(const std::string& version_name) {
        return make_api_call("/version/" + version_name,"GET");
    }

    // Version Groups
    pplx::task<json::value> get_version_group_list(int limit = 20, int offset = 0) {
        std::string endpoint = "/version-group?limit=" + std::to_string(limit) + 
                              "&offset=" + std::to_string(offset);
        return make_api_call(endpoint,"GET");
    }

    pplx::task<json::value> get_version_group_by_id(int version_group_id) {
        return make_api_call("/version-group/" + std::to_string(version_group_id),"GET");
    }

    pplx::task<json::value> get_version_group_by_name(const std::string& version_group_name) {
        return make_api_call("/version-group/" + version_group_name,"GET");
    }

    // Items
    pplx::task<json::value> get_item_list(int limit = 20, int offset = 0) {
        std::string endpoint = "/item?limit=" + std::to_string(limit) + 
                              "&offset=" + std::to_string(offset);
        return make_api_call(endpoint,"GET");
    }

    pplx::task<json::value> get_item_by_id(int item_id) {
        return make_api_call("/item/" + std::to_string(item_id),"GET");
    }

    pplx::task<json::value> get_item_by_name(const std::string& item_name) {
        return make_api_call("/item/" + item_name,"GET");
    }

    // Item Attributes
    pplx::task<json::value> get_item_attribute_list(int limit = 20, int offset = 0) {
        std::string endpoint = "/item-attribute?limit=" + std::to_string(limit) + 
                              "&offset=" + std::to_string(offset);
        return make_api_call(endpoint,"GET");
    }

    pplx::task<json::value> get_item_attribute_by_id(int attribute_id) {
        return make_api_call("/item-attribute/" + std::to_string(attribute_id),"GET");
    }

    pplx::task<json::value> get_item_attribute_by_name(const std::string& attribute_name) {
        return make_api_call("/item-attribute/" + attribute_name,"GET");
    }

    // Item Categories
    pplx::task<json::value> get_item_category_list(int limit = 20, int offset = 0) {
        std::string endpoint = "/item-category?limit=" + std::to_string(limit) + 
                              "&offset=" + std::to_string(offset);
        return make_api_call(endpoint,"GET");
    }

    pplx::task<json::value> get_item_category_by_id(int category_id) {
        return make_api_call("/item-category/" + std::to_string(category_id),"GET");
    }

    pplx::task<json::value> get_item_category_by_name(const std::string& category_name) {
        return make_api_call("/item-category/" + category_name,"GET");
    }

    // Item Fling Effects
    pplx::task<json::value> get_item_fling_effect_list(int limit = 20, int offset = 0) {
        std::string endpoint = "/item-fling-effect?limit=" + std::to_string(limit) + 
                              "&offset=" + std::to_string(offset);
        return make_api_call(endpoint,"GET");
    }

    pplx::task<json::value> get_item_fling_effect_by_id(int effect_id) {
        return make_api_call("/item-fling-effect/" + std::to_string(effect_id),"GET");
    }

    pplx::task<json::value> get_item_fling_effect_by_name(const std::string& effect_name) {
        return make_api_call("/item-fling-effect/" + effect_name,"GET");
    }

    // Item Pockets
    pplx::task<json::value> get_item_pocket_list(int limit = 20, int offset = 0) {
        std::string endpoint = "/item-pocket?limit=" + std::to_string(limit) + 
                              "&offset=" + std::to_string(offset);
        return make_api_call(endpoint,"GET");
    }

    pplx::task<json::value> get_item_pocket_by_id(int pocket_id) {
        return make_api_call("/item-pocket/" + std::to_string(pocket_id),"GET");
    }

    pplx::task<json::value> get_item_pocket_by_name(const std::string& pocket_name) {
        return make_api_call("/item-pocket/" + pocket_name,"GET");
    }

    // Locations
    pplx::task<json::value> get_location_list(int limit = 20, int offset = 0) {
        std::string endpoint = "/location?limit=" + std::to_string(limit) + 
                              "&offset=" + std::to_string(offset);
        return make_api_call(endpoint,"GET");
    }

    pplx::task<json::value> get_location_by_id(int location_id) {
        return make_api_call("/location/" + std::to_string(location_id),"GET");
    }

    pplx::task<json::value> get_location_by_name(const std::string& location_name) {
        return make_api_call("/location/" + location_name,"GET");
    }

    // Location Areas
    pplx::task<json::value> get_location_area_list(int limit = 20, int offset = 0) {
        std::string endpoint = "/location-area?limit=" + std::to_string(limit) + 
                              "&offset=" + std::to_string(offset);
        return make_api_call(endpoint,"GET");
    }

    pplx::task<json::value> get_location_area_by_id(int area_id) {
        return make_api_call("/location-area/" + std::to_string(area_id),"GET");
    }

    pplx::task<json::value> get_location_area_by_name(const std::string& area_name) {
        return make_api_call("/location-area/" + area_name,"GET");
    }

    // Pal Park Areas
    pplx::task<json::value> get_pal_park_area_list(int limit = 20, int offset = 0) {
        std::string endpoint = "/pal-park-area?limit=" + std::to_string(limit) + 
                              "&offset=" + std::to_string(offset);
        return make_api_call(endpoint,"GET");
    }

    pplx::task<json::value> get_pal_park_area_by_id(int area_id) {
        return make_api_call("/pal-park-area/" + std::to_string(area_id),"GET");
    }

    pplx::task<json::value> get_pal_park_area_by_name(const std::string& area_name) {
        return make_api_call("/pal-park-area/" + area_name,"GET");
    }

    // Regions
    pplx::task<json::value> get_region_list(int limit = 20, int offset = 0) {
        std::string endpoint = "/region?limit=" + std::to_string(limit) + 
                              "&offset=" + std::to_string(offset);
        return make_api_call(endpoint,"GET");
    }

    pplx::task<json::value> get_region_by_id(int region_id) {
        return make_api_call("/region/" + std::to_string(region_id),"GET");
    }

    pplx::task<json::value> get_region_by_name(const std::string& region_name) {
        return make_api_call("/region/" + region_name,"GET");
    }

    pplx::task<json::value> get_machine_list(int limit = 20, int offset = 0) {
        std::string endpoint = "/machine?limit=" + std::to_string(limit) + 
                              "&offset=" + std::to_string(offset);
        return make_api_call(endpoint,"GET");
    }

    pplx::task<json::value> get_machine_by_id(int machine_id) {
        return make_api_call("/machine/" + std::to_string(machine_id),"GET");
    }

    // Moves
    pplx::task<json::value> get_move_list(int limit = 20, int offset = 0) {
        std::string endpoint = "/move?limit=" + std::to_string(limit) + 
                              "&offset=" + std::to_string(offset);
        return make_api_call(endpoint,"GET");
    }

    pplx::task<json::value> get_move_by_id(int move_id) {
        return make_api_call("/move/" + std::to_string(move_id),"GET");
    }

    pplx::task<json::value> get_move_by_name(const std::string& move_name) {
        return make_api_call("/move/" + move_name,"GET");
    }

    // Move Ailments
    pplx::task<json::value> get_move_ailment_list(int limit = 20, int offset = 0) {
        std::string endpoint = "/move-ailment?limit=" + std::to_string(limit) + 
                              "&offset=" + std::to_string(offset);
        return make_api_call(endpoint,"GET");
    }

    pplx::task<json::value> get_move_ailment_by_id(int ailment_id) {
        return make_api_call("/move-ailment/" + std::to_string(ailment_id),"GET");
    }

    pplx::task<json::value> get_move_ailment_by_name(const std::string& ailment_name) {
        return make_api_call("/move-ailment/" + ailment_name,"GET");
    }

    // Move Battle Styles
    pplx::task<json::value> get_move_battle_style_list(int limit = 20, int offset = 0) {
        std::string endpoint = "/move-battle-style?limit=" + std::to_string(limit) + 
                              "&offset=" + std::to_string(offset);
        return make_api_call(endpoint,"GET");
    }

    pplx::task<json::value> get_move_battle_style_by_id(int style_id) {
        return make_api_call("/move-battle-style/" + std::to_string(style_id),"GET");
    }

    pplx::task<json::value> get_move_battle_style_by_name(const std::string& style_name) {
        return make_api_call("/move-battle-style/" + style_name,"GET");
    }

    // Move Categories
    pplx::task<json::value> get_move_category_list(int limit = 20, int offset = 0) {
        std::string endpoint = "/move-category?limit=" + std::to_string(limit) + 
                              "&offset=" + std::to_string(offset);
        return make_api_call(endpoint,"GET");
    }

    pplx::task<json::value> get_move_category_by_id(int category_id) {
        return make_api_call("/move-category/" + std::to_string(category_id),"GET");
    }

    pplx::task<json::value> get_move_category_by_name(const std::string& category_name) {
        return make_api_call("/move-category/" + category_name,"GET");
    }

    // Move Damage Classes
    pplx::task<json::value> get_move_damage_class_list(int limit = 20, int offset = 0) {
        std::string endpoint = "/move-damage-class?limit=" + std::to_string(limit) + 
                              "&offset=" + std::to_string(offset);
        return make_api_call(endpoint,"GET");
    }

    pplx::task<json::value> get_move_damage_class_by_id(int damage_class_id) {
        return make_api_call("/move-damage-class/" + std::to_string(damage_class_id),"GET");
    }

    pplx::task<json::value> get_move_damage_class_by_name(const std::string& damage_class_name) {
        return make_api_call("/move-damage-class/" + damage_class_name,"GET");
    }

    // Move Learn Methods
    pplx::task<json::value> get_move_learn_method_list(int limit = 20, int offset = 0) {
        std::string endpoint = "/move-learn-method?limit=" + std::to_string(limit) + 
                              "&offset=" + std::to_string(offset);
        return make_api_call(endpoint,"GET");
    }

    pplx::task<json::value> get_move_learn_method_by_id(int method_id) {
        return make_api_call("/move-learn-method/" + std::to_string(method_id),"GET");
    }

    pplx::task<json::value> get_move_learn_method_by_name(const std::string& method_name) {
        return make_api_call("/move-learn-method/" + method_name,"GET");
    }

    // Move Targets
    pplx::task<json::value> get_move_target_list(int limit = 20, int offset = 0) {
        std::string endpoint = "/move-target?limit=" + std::to_string(limit) + 
                              "&offset=" + std::to_string(offset);
        return make_api_call(endpoint,"GET");
    }

    pplx::task<json::value> get_move_target_by_id(int target_id) {
        return make_api_call("/move-target/" + std::to_string(target_id),"GET");
    }

    pplx::task<json::value> get_move_target_by_name(const std::string& target_name) {
        return make_api_call("/move-target/" + target_name,"GET");
    }

    // Abilities
    pplx::task<json::value> get_ability_list(int limit = 20, int offset = 0) {
        std::string endpoint = "/ability?limit=" + std::to_string(limit) + 
                              "&offset=" + std::to_string(offset);
        return make_api_call(endpoint,"GET");
    }

    pplx::task<json::value> get_ability_by_id(int ability_id) {
        return make_api_call("/ability/" + std::to_string(ability_id),"GET");
    }

    pplx::task<json::value> get_ability_by_name(const std::string& ability_name) {
        return make_api_call("/ability/" + ability_name,"GET");
    }

    // Characteristics
    pplx::task<json::value> get_characteristic_list(int limit = 20, int offset = 0) {
        std::string endpoint = "/characteristic?limit=" + std::to_string(limit) + 
                              "&offset=" + std::to_string(offset);
        return make_api_call(endpoint,"GET");
    }

    pplx::task<json::value> get_characteristic_by_id(int characteristic_id) {
        return make_api_call("/characteristic/" + std::to_string(characteristic_id),"GET");
    }

    // Egg Groups
    pplx::task<json::value> get_egg_group_list(int limit = 20, int offset = 0) {
        std::string endpoint = "/egg-group?limit=" + std::to_string(limit) + 
                              "&offset=" + std::to_string(offset);
        return make_api_call(endpoint,"GET");
    }

    pplx::task<json::value> get_egg_group_by_id(int egg_group_id) {
        return make_api_call("/egg-group/" + std::to_string(egg_group_id),"GET");
    }

    pplx::task<json::value> get_egg_group_by_name(const std::string& egg_group_name) {
        return make_api_call("/egg-group/" + egg_group_name,"GET");
    }

    // Genders
    pplx::task<json::value> get_gender_list(int limit = 20, int offset = 0) {
        std::string endpoint = "/gender?limit=" + std::to_string(limit) + 
                              "&offset=" + std::to_string(offset);
        return make_api_call(endpoint,"GET");
    }

    pplx::task<json::value> get_gender_by_id(int gender_id) {
        return make_api_call("/gender/" + std::to_string(gender_id),"GET");
    }

    pplx::task<json::value> get_gender_by_name(const std::string& gender_name) {
        return make_api_call("/gender/" + gender_name,"GET");
    }

    // Growth Rates
    pplx::task<json::value> get_growth_rate_list(int limit = 20, int offset = 0) {
        std::string endpoint = "/growth-rate?limit=" + std::to_string(limit) + 
                              "&offset=" + std::to_string(offset);
        return make_api_call(endpoint,"GET");
    }

    pplx::task<json::value> get_growth_rate_by_id(int growth_rate_id) {
        return make_api_call("/growth-rate/" + std::to_string(growth_rate_id),"GET");
    }

    pplx::task<json::value> get_growth_rate_by_name(const std::string& growth_rate_name) {
        return make_api_call("/growth-rate/" + growth_rate_name,"GET");
    }

    // Natures
    pplx::task<json::value> get_nature_list(int limit = 20, int offset = 0) {
        std::string endpoint = "/nature?limit=" + std::to_string(limit) + 
                              "&offset=" + std::to_string(offset);
        return make_api_call(endpoint,"GET");
    }

    pplx::task<json::value> get_nature_by_id(int nature_id) {
        return make_api_call("/nature/" + std::to_string(nature_id),"GET");
    }

    pplx::task<json::value> get_nature_by_name(const std::string& nature_name) {
        return make_api_call("/nature/" + nature_name,"GET");
    }

    // Pokeathlon Stats
    pplx::task<json::value> get_pokeathlon_stat_list(int limit = 20, int offset = 0) {
        std::string endpoint = "/pokeathlon-stat?limit=" + std::to_string(limit) + 
                              "&offset=" + std::to_string(offset);
        return make_api_call(endpoint,"GET");
    }

    pplx::task<json::value> get_pokeathlon_stat_by_id(int stat_id) {
        return make_api_call("/pokeathlon-stat/" + std::to_string(stat_id),"GET");
    }

    pplx::task<json::value> get_pokeathlon_stat_by_name(const std::string& stat_name) {
        return make_api_call("/pokeathlon-stat/" + stat_name,"GET");
    }

    // Pokemon
    pplx::task<json::value> get_pokemon_list(int limit = 20, int offset = 0) {
        std::string endpoint = "/pokemon?limit=" + std::to_string(limit) + 
                              "&offset=" + std::to_string(offset);
        return make_api_call(endpoint,"GET");
    }

    pplx::task<json::value> get_pokemon_by_id(int pokemon_id) {
        return make_api_call("/pokemon/" + std::to_string(pokemon_id),"GET");
    }

    pplx::task<json::value> get_pokemon_by_name(const std::string& pokemon_name) {
        return make_api_call("/pokemon/" + pokemon_name,"GET");
    }

    // Pokemon Location Areas
    pplx::task<json::value> get_pokemon_location_areas(int pokemon_id) {
        return make_api_call("/pokemon/" + std::to_string(pokemon_id) + "/encounters","GET");
    }

    pplx::task<json::value> get_pokemon_location_areas_by_name(const std::string& pokemon_name) {
        return make_api_call("/pokemon/" + pokemon_name + "/encounters","GET");
    }

    // Pokemon Colors
    pplx::task<json::value> get_pokemon_color_list(int limit = 20, int offset = 0) {
        std::string endpoint = "/pokemon-color?limit=" + std::to_string(limit) + 
                              "&offset=" + std::to_string(offset);
        return make_api_call(endpoint,"GET");
    }

    pplx::task<json::value> get_pokemon_color_by_id(int color_id) {
        return make_api_call("/pokemon-color/" + std::to_string(color_id),"GET");
    }

    pplx::task<json::value> get_pokemon_color_by_name(const std::string& color_name) {
        return make_api_call("/pokemon-color/" + color_name,"GET");
    }

    // Pokemon Forms
    pplx::task<json::value> get_pokemon_form_list(int limit = 20, int offset = 0) {
        std::string endpoint = "/pokemon-form?limit=" + std::to_string(limit) + 
                              "&offset=" + std::to_string(offset);
        return make_api_call(endpoint,"GET");
    }

    pplx::task<json::value> get_pokemon_form_by_id(int form_id) {
        return make_api_call("/pokemon-form/" + std::to_string(form_id),"GET");
    }

    pplx::task<json::value> get_pokemon_form_by_name(const std::string& form_name) {
        return make_api_call("/pokemon-form/" + form_name,"GET");
    }

    // Pokemon Habitats
    pplx::task<json::value> get_pokemon_habitat_list(int limit = 20, int offset = 0) {
        std::string endpoint = "/pokemon-habitat?limit=" + std::to_string(limit) + 
                              "&offset=" + std::to_string(offset);
        return make_api_call(endpoint,"GET");
    }

    pplx::task<json::value> get_pokemon_habitat_by_id(int habitat_id) {
        return make_api_call("/pokemon-habitat/" + std::to_string(habitat_id),"GET");
    }

    pplx::task<json::value> get_pokemon_habitat_by_name(const std::string& habitat_name) {
        return make_api_call("/pokemon-habitat/" + habitat_name,"GET");
    }

    // Pokemon Shapes
    pplx::task<json::value> get_pokemon_shape_list(int limit = 20, int offset = 0) {
        std::string endpoint = "/pokemon-shape?limit=" + std::to_string(limit) + 
                              "&offset=" + std::to_string(offset);
        return make_api_call(endpoint,"GET");
    }

    pplx::task<json::value> get_pokemon_shape_by_id(int shape_id) {
        return make_api_call("/pokemon-shape/" + std::to_string(shape_id),"GET");
    }

    pplx::task<json::value> get_pokemon_shape_by_name(const std::string& shape_name) {
        return make_api_call("/pokemon-shape/" + shape_name,"GET");
    }

    // Pokemon Species
    pplx::task<json::value> get_pokemon_species_list(int limit = 20, int offset = 0) {
        std::string endpoint = "/pokemon-species?limit=" + std::to_string(limit) + 
                              "&offset=" + std::to_string(offset);
        return make_api_call(endpoint,"GET");
    }

    pplx::task<json::value> get_pokemon_species_by_id(int species_id) {
        return make_api_call("/pokemon-species/" + std::to_string(species_id),"GET");
    }

    pplx::task<json::value> get_pokemon_species_by_name(const std::string& species_name) {
        return make_api_call("/pokemon-species/" + species_name,"GET");
    }

    // Stats
    pplx::task<json::value> get_stat_list(int limit = 20, int offset = 0) {
        std::string endpoint = "/stat?limit=" + std::to_string(limit) + 
                              "&offset=" + std::to_string(offset);
        return make_api_call(endpoint,"GET");
    }

    pplx::task<json::value> get_stat_by_id(int stat_id) {
        return make_api_call("/stat/" + std::to_string(stat_id),"GET");
    }

    pplx::task<json::value> get_stat_by_name(const std::string& stat_name) {
        return make_api_call("/stat/" + stat_name,"GET");
    }

    // Types
    pplx::task<json::value> get_type_list(int limit = 20, int offset = 0) {
        std::string endpoint = "/type?limit=" + std::to_string(limit) + 
                              "&offset=" + std::to_string(offset);
        return make_api_call(endpoint,"GET");
    }

    pplx::task<json::value> get_type_by_id(int type_id) {
        return make_api_call("/type/" + std::to_string(type_id),"GET");
    }

    pplx::task<json::value> get_type_by_name(const std::string& type_name) {
        return make_api_call("/type/" + type_name,"GET");
    }

    // Languages
    pplx::task<json::value> get_language_list(int limit = 20, int offset = 0) {
        std::string endpoint = "/language?limit=" + std::to_string(limit) + 
                              "&offset=" + std::to_string(offset);
        return make_api_call(endpoint,"GET");
    }

    pplx::task<json::value> get_language_by_id(int language_id) {
        return make_api_call("/language/" + std::to_string(language_id),"GET");
    }

    pplx::task<json::value> get_language_by_name(const std::string& language_name) {
        return make_api_call("/language/" + language_name,"GET");
    }

    pplx::task<json::value> get_berry_list(int limit = 20, int offset = 0) {
        std::string endpoint = "/berry?limit=" + std::to_string(limit) + 
                          "&offset=" + std::to_string(offset);
        return make_api_call(endpoint,"GET");
    }

    pplx::task<json::value> get_berry_by_id(int berry_id) {
        return make_api_call("/berry/" + std::to_string(berry_id),"GET");
    }

    pplx::task<json::value> get_berry_by_name(const std::string& berry_name) {
        return make_api_call("/berry/" + berry_name,"GET");
    }

// Berry Firmnesses
    pplx::task<json::value> get_berry_firmness_list(int limit = 20, int offset = 0) {
        std::string endpoint = "/berry-firmness?limit=" + std::to_string(limit) + 
                          "&offset=" + std::to_string(offset);
        return make_api_call(endpoint,"GET");
    }

    pplx::task<json::value> get_berry_firmness_by_id(int firmness_id) {
        return make_api_call("/berry-firmness/" + std::to_string(firmness_id),"GET");
    }

    pplx::task<json::value> get_berry_firmness_by_name(const std::string& firmness_name) {
        return make_api_call("/berry-firmness/" + firmness_name,"GET");
    }

// Berry Flavors
    pplx::task<json::value> get_berry_flavor_list(int limit = 20, int offset = 0) {
        std::string endpoint = "/berry-flavor?limit=" + std::to_string(limit) + 
                          "&offset=" + std::to_string(offset);
        return make_api_call(endpoint,"GET");
    }

    pplx::task<json::value> get_berry_flavor_by_id(int flavor_id) {
        return make_api_call("/berry-flavor/" + std::to_string(flavor_id),"GET");
    }

    pplx::task<json::value> get_berry_flavor_by_name(const std::string& flavor_name) {
        return make_api_call("/berry-flavor/" + flavor_name,"GET");
    }

// Contest Types
    pplx::task<json::value> get_contest_type_list(int limit = 20, int offset = 0) {
        std::string endpoint = "/contest-type?limit=" + std::to_string(limit) + 
                          "&offset=" + std::to_string(offset);
        return make_api_call(endpoint,"GET");
    }

    pplx::task<json::value> get_contest_type_by_id(int contest_type_id) {
        return make_api_call("/contest-type/" + std::to_string(contest_type_id),"GET");
    }

    pplx::task<json::value> get_contest_type_by_name(const std::string& contest_type_name) {
        return make_api_call("/contest-type/" + contest_type_name,"GET");
    }

// Contest Effects
    pplx::task<json::value> get_contest_effect_list(int limit = 20, int offset = 0) {
        std::string endpoint = "/contest-effect?limit=" + std::to_string(limit) + 
                          "&offset=" + std::to_string(offset);
        return make_api_call(endpoint,"GET");
    }

    pplx::task<json::value> get_contest_effect_by_id(int effect_id) {
        return make_api_call("/contest-effect/" + std::to_string(effect_id),"GET");
    }

// Super Contest Effects
    pplx::task<json::value> get_super_contest_effect_list(int limit = 20, int offset = 0) {
        std::string endpoint = "/super-contest-effect?limit=" + std::to_string(limit) + 
                          "&offset=" + std::to_string(offset);
        return make_api_call(endpoint,"GET");
    }

    pplx::task<json::value> get_super_contest_effect_by_id(int effect_id) {
        return make_api_call("/super-contest-effect/" + std::to_string(effect_id),"GET");
    }

// Encounter Methods
    pplx::task<json::value> get_encounter_method_list(int limit = 20, int offset = 0) {
        std::string endpoint = "/encounter-method?limit=" + std::to_string(limit) + 
                          "&offset=" + std::to_string(offset);
        return make_api_call(endpoint,"GET");
    }

    pplx::task<json::value> get_encounter_method_by_id(int method_id) {
        return make_api_call("/encounter-method/" + std::to_string(method_id),"GET");
    }

    pplx::task<json::value> get_encounter_method_by_name(const std::string& method_name) {
        return make_api_call("/encounter-method/" + method_name,"GET");
    }

};

#endif
