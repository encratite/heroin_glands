#ifndef ITEM_HPP
#define ITEM_HPP

#include <vector>

#include <heroin/game.hpp>

//stolen from Awesom-O basically, I like the namespace prefixing, copying it, added using directives

namespace item_version
{
	enum item_version_type
	{
		classic = 0,
		classic110 = 2,
		lod = 100,
		lod110 = 101
	};
}

namespace item_class
{
	enum item_class_type
	{
		not_applicable,
		normal,
		exceptional,
		elite
	};
}

namespace item_quality
{
	enum item_quality_type
	{
		not_applicable,
		inferior,
		normal,
		superior,
		magical,
		set,
		rare,
		unique,
		crafted
	};
};

namespace equipment_directory
{
	enum equipment_directory_type
	{
		not_applicable = 0,
		helm = 1,
		amulet = 2,
		armor = 3,
		right_hand = 4,
		left_hand = 5,
		right_hand_ring = 6,
		left_hand_ring = 7,
		belt = 8,
		boots = 9,
		gloves = 10,
		right_hand_switch = 11,
		left_hand_switch = 12
	};
}

namespace item_container
{
	enum item_container_type
	{
		unspecified = 0,
		inventory = 2,
		trader_offer = 4,
		for_trade = 6,
		cube = 8,
		stash = 0x0A,
		belt = 0x20,
		item = 0x40,
		armor_tab = 0x82,
		weapon_tab_1 = 0x84,
		weapon_tab_2 = 0x86,
		misc_tab = 0x88,
	};
}

namespace item_action
{
	enum item_action_type
	{
		add_to_ground = 0,
		ground_to_cursor = 1,
		drop_to_ground = 2,
		on_ground = 3,
		put_in_container = 4,
		remove_from_container = 5,
		equip = 6,
		indirectly_swap_body_item = 7,
		unequip = 8,
		swap_body_item = 9,
		add_quantity = 0x0A,
		add_to_shop = 0x0B,
		remove_from_shop = 0x0C,
		swap_in_container = 0x0D,
		put_in_belt = 0x0E,
		remove_from_belt = 0x0F,
		swap_in_belt = 0x10,
		auto_unequip = 0x11,
		to_cursor = 0x12,
		item_in_socket = 0x13,
		update_stats = 0x15,
		weapon_switch = 0x17
	};
}

namespace item_container_gc
{
	enum item_container_gc_type
	{
		inventory = 0,
		trade = 2,
		cube = 3,
		stash = 4,
	};
}

namespace item_destination
{
	enum item_destination_type
	{
		unspecified = 0,
		equipment = 1,
		belt = 2,
		ground = 3,
		cursor = 4,
		item = 6,
	};
}

namespace superior_item_class
{
	enum superior_item_class_type
	{
		not_applicable = -1,
		attackRating = 0,
		damage = 1,
		defense = 2,
		damage_attack_rating = 3,
		durability = 4,
		attack_rating_durability = 5,
		damage_durability = 6,
		defense_durability = 7
	};
}

namespace item_classification
{
	enum item_classification_type
	{
		amazon_bow,
		amazon_javelin,
		amazon_spear,
		amulet,
		antidote_potion,
		armor,
		arrows,
		assassin_katar,
		axe,
		barbarian_helm,
		belt,
		body_part,
		bolts,
		boots,
		bow,
		circlet,
		club,
		crossbow,
		dagger,
		druid_pelt,
		ear,
		elixir,
		gem,
		gloves,
		gold,
		grand_charm,
		hammer,
		health_potion,
		helm,
		herb,
		javelin,
		jewel,
		key,
		large_charm,
		mace,
		mana_potion,
		necromancer_shrunken_head,
		paladin_shield,
		polearm,
		quest_item,
		rejuvenation_potion,
		ring,
		rune,
		scepter,
		scroll,
		shield,
		small_charm,
		sorceress_orb,
		spear,
		staff,
		stamina_potion,
		sword,
		thawing_potion,
		throwing_axe,
		throwing_knife,
		throwing_potion,
		tome,
		torch,
		wand
	};
}

namespace item_stat
{
	enum item_stat_type
	{
		strength,
		energy,
		dexterity,
		vitality,
		all_attributes,
		new_skills,
		life,
		maximum_life,
		mana,
		maximum_mana,
		stamina,
		maximum_stamina,
		level,
		experience,
		gold,
		bank,
		enhanced_defense,
		enhanced_maximum_damage,
		enhanced_minimum_damage,
		attack_rating,
		increased_blocking,
		minimum_damage,
		maximum_damage,
		secondary_minimum_damage,
		secondary_maximum_damage,
		enhanced_damage,
		mana_recovery,
		mana_recovery_bonus,
		stamina_recovery_bonus,
		last_experience,
		next_experience,
		defense,
		defense_vs_missiles,
		defense_vs_melee,
		damage_reduction,
		magical_damage_reduction,
		damage_reduction_percent,
		magical_damage_reduction_percent,
		maximum_magical_damage_reduction_percent,
		fire_resistance,
		maximum_fire_resistance,
		lightning_resistance,
		maximum_lightning_resistance,
		cold_resistance,
		maximum_cold_resistance,
		poison_resistance,
		maximum_poison_resistance,
		damage_aura,
		minimum_fire_damage,
		maximum_fire_damage,
		minimum_lightning_damage,
		maximum_lightning_damage,
		minimum_magical_damage,
		maximum_magical_damage,
		minimum_cold_damage,
		maximum_cold_damage,
		cold_damage_length,
		minimum_poison_damage,
		maximum_poison_damage,
		poison_damage_length,
		minimum_life_stolen_per_hit,
		maximum_life_stolen_per_hit,
		minimum_mana_stolen_per_hit,
		maximum_mana_stolen_per_hit,
		minimum_stamina_drain,
		maximum_stamina_drain,
		stun_length,
		velocity_percent,
		attack_rate,
		other_animation_rate,
		quantity,
		value,
		durability,
		maximum_durability,
		replenish_life,
		enhanced_maximum_durability,
		enhanced_life,
		enhanced_mana,
		attacker_takes_damage,
		extra_gold,
		better_chance_of_getting_magic_item,
		knockback,
		time_duration,
		class_skills,
		unsent_parameter,
		add_experience,
		life_after_each_kill,
		reduce_vendor_prices,
		double_herb_duration,
		light_radius,
		light_colour,
		reduced_requirements,
		reduced_level_requirement,
		increased_attack_speed,
		reduced_level_requirement_percent,
		last_block_frame,
		faster_run_walk,
		non_class_skill,
		state,
		faster_hit_recovery,
		monster_player_count,
		skill_poison_override_length,
		faster_block_rate,
		skill_bypass_undead,
		skill_bypass_demons,
		faster_cast_rate,
		skill_bypass_beasts,
		single_skill,
		slain_monsters_rest_in_peace,
		curse_resistance,
		poison_length_reduction,
		adds_damage,
		hit_causes_monster_to_flee,
		hit_blinds_target,
		damage_to_mana,
		ignore_targets_defense,
		reduce_targets_defense,
		prevent_monster_heal,
		half_freeze_duration,
		to_hit_percent,
		monster_defense_reduction_per_hit,
		damage_to_demons,
		damage_to_undead,
		attack_rating_against_demons,
		attack_rating_against_undead,
		throwable,
		elemental_skills,
		all_skills,
		attacker_takes_lightning_damage,
		iron_maiden_level,
		lifetap_level,
		thorns_percent,
		bone_armor,
		maximum_bone_armor,
		freezes_target,
		open_wounds,
		crushing_blow,
		kick_damage,
		mana_after_each_kill,
		life_after_each_demon_kill,
		extra_blood,
		deadly_strike,
		fire_absorption_percent,
		fire_absorption,
		lightning_absorption_percent,
		lightning_absorption,
		magic_absorption_percent,
		magic_absorption,
		cold_absorption_percent,
		cold_absorption,
		slows_down_enemies,
		aura,
		indestructible,
		cannot_be_frozen,
		stamina_drain_percent,
		reanimate,
		piercing_attack,
		fires_magic_arrows,
		fire_explosive_arrows,
		minimum_throwing_damage,
		maximum_throwing_damage,
		skill_hand_of_athena,
		skill_stamina_percent,
		skill_passive_stamina_percent,
		concentration,
		enchant,
		pierce,
		conviction,
		chilling_armor,
		frenzy,
		decrepify,
		skill_armor_percent,
		alignment,
		target_0,
		target_1,
		gold_lost,
		conversion_level,
		conversion_maximum_life,
		unit_do_overlay,
		attack_rating_against_monster_type,
		damage_to_monster_type,
		fade,
		armor_override_percent,
		unused_183,
		unused_184,
		unused_185,
		unused_186,
		unused_187,
		skill_tab,
		unused_189,
		unused_190,
		unused_191,
		unused_192,
		unused_193,
		socket_count,
		skill_on_striking,
		skill_on_kill,
		skill_on_death,
		skill_on_hit,
		skill_on_level_up,
		unused_200,
		skill_when_struck,
		unused_202,
		unused_203,
		charged,
		unused_204,
		unused_205,
		unused_206,
		unused_207,
		unused_208,
		unused_209,
		unused_210,
		unused_211,
		unused_212,
		defense_per_level,
		enhanced_defense_per_level,
		life_per_level,
		mana_per_level,
		maximum_damage_per_level,
		maximum_enhanced_damage_per_level,
		strength_per_level,
		dexterity_per_level,
		energy_per_level,
		vitality_per_level,
		attack_rating_per_level,
		bonus_to_attack_rating_per_level,
		maximum_cold_damage_per_level,
		maximum_fire_damage_per_level,
		maximum_lightning_damage_per_level,
		maximum_poison_damage_per_level,
		cold_resistance_per_level,
		fire_resistance_per_level,
		lightning_resistance_per_level,
		poison_resistance_per_level,
		cold_absorption_per_level,
		fire_absorption_per_level,
		lightning_absorption_per_level,
		poison_absorption_per_level,
		thorns_per_level,
		extra_gold_per_level,
		better_chance_of_getting_magic_item_per_level,
		stamina_regeneration_per_level,
		stamina_per_level,
		damage_to_demons_per_level,
		damage_to_undead_per_level,
		attack_rating_against_demons_per_level,
		attack_rating_against_undead_per_level,
		crushing_blow_per_level,
		open_wounds_per_level,
		kick_damage_per_level,
		deadly_strike_per_level,
		find_gems_per_level,
		repairs_durability,
		replenishes_quantity,
		increased_stack_size,
		find_item,
		slash_damage,
		slash_damage_percent,
		crush_damage,
		crush_damage_percent,
		thrust_damage,
		thrust_damage_percent,
		slash_damage_absorption,
		crush_damage_absorption,
		thrust_damage_absorption,
		slash_damage_absorption_percent,
		crush_damage_absorption_percent,
		thrust_damage_absorption_percent,
		defense_per_time,
		enhanced_defense_per_time,
		life_per_time,
		mana_per_time,
		maximum_damage_per_time,
		maximum_enhanced_damage_per_time,
		strength_per_time,
		dexterity_per_time,
		energy_per_time,
		vitality_per_time,
		attack_rating_per_time,
		chance_to_hit_per_time,
		maximum_cold_damage_per_time,
		maximum_fire_damage_per_time,
		maximum_lightning_damage_per_time,
		maximum_damage_per_poison,
		cold_resistance_per_time,
		fire_resistance_per_time,
		lightning_resistance_per_time,
		poison_resistance_per_time,
		cold_absorption_per_time,
		fire_absorption_per_time,
		lightning_absorption_per_time,
		poison_absorption_per_time,
		extra_gold_per_time,
		better_chance_of_getting_magic_item_per_time,
		regenerate_stamina_per_time,
		stamina_per_time,
		damage_to_demons_per_time,
		damage_to_undead_per_time,
		attack_rating_against_demons_per_time,
		attack_rating_against_undead_per_time,
		crushing_blow_per_time,
		open_wounds_per_time,
		kick_damage_per_time,
		deadly_strike_per_time,
		find_gems_per_time,
		enemy_cold_resistance_reduction,
		enemy_fire_resistance_reduction,
		enemy_lightning_resistance_reduction,
		enemy_poison_resistance_reduction,
		damage_vs_monsters,
		enhanced_damage_vs_monsters,
		attack_rating_against_monsters,
		bonus_to_attack_rating_against_monsters,
		defense_vs_monsters,
		enhanced_defense_vs_monsters,
		fire_damage_length,
		minimum_fire_damage_length,
		maximum_fire_damage_length,
		progressive_damage,
		progressive_steal,
		progressive_other,
		progressive_fire,
		progressive_cold,
		progressive_lightning,
		extra_charges,
		progressive_attack_rating,
		poison_count,
		damage_framerate,
		pierce_idx,
		fire_mastery,
		lightning_mastery,
		cold_mastery,
		poison_mastery,
		passive_enemy_fire_resistance_reduction,
		passive_enemy_lightning_resistance_reduction,
		passive_enemy_cold_resistance_reduction,
		passive_enemy_poison_resistance_reduction,
		critical_strike,
		dodge,
		avoid,
		evade,
		warmth,
		melee_attack_rating_mastery,
		melee_damage_mastery,
		melee_critical_hit_mastery,
		thrown_weapon_attack_rating_mastery,
		thrown_weapon_damage_mastery,
		thrown_weapon_critical_hit_mastery,
		weapon_block,
		summon_resist,
		modifier_list_skill,
		modifier_list_level,
		last_sent_life_percent,
		source_unit_type,
		source_unit_id,
		short_parameter_1,
		quest_item_difficulty,
		passive_magical_damage_mastery,
		passive_magical_resistance_reduction
	};
}

extern std::string const item_classification_strings[];

namespace item_code
{
	extern std::string const
		gold;
}

using item_version::item_version_type;
using item_class::item_class_type;
using item_quality::item_quality_type;
using equipment_directory::equipment_directory_type;
using item_container::item_container_type;
using item_action::item_action_type;
using item_container_gc::item_container_gc_type;
using item_destination::item_destination_type;
using superior_item_class::superior_item_class_type;
using item_classification::item_classification_type;
using item_stat::item_stat_type;

struct item_property_type
{
	item_stat_type stat;
	long value;

	ulong minimum;
	ulong maximum;
	ulong length;

	ulong level;
	ulong character_class;
	ulong skill;
	ulong tab;

	ulong monster;

	ulong charges;
	ulong maximum_charges;

	ulong skill_chance;

	ulong per_level;

	item_property_type();
};

struct item_type
{
	//debugging
	std::string packet;

	ulong action;

	std::string name;

	ulong width;
	ulong height;

	ulong category;
	ulong id;
	bool equipped;
	bool in_socket;
	bool identified;
	bool switched_in;
	bool switched_out;
	bool broken;
	bool potion;
	bool has_sockets;
	bool in_store;
	bool not_in_a_socket;
	bool ear;
	bool start_item;
	bool simple_item;
	bool ethereal;
	bool personalised;
	bool gambling;
	bool rune_word;

	bool ground;

	item_version_type version;

	bool unspecified_directory;
	ulong x;
	ulong y;
	ulong directory;
	item_container_type container;

	character_class_type ear_character_class;
	ulong ear_level;
	std::string ear_name;

	std::string type;

	bool is_gold;
	ulong amount;

	ulong used_sockets;
	ulong level;
	item_quality_type quality;

	bool has_graphic;
	ulong graphic;

	bool has_colour;
	ulong colour;

	ulong prefix;
	ulong suffix;

	std::vector<ulong> prefixes;
	std::vector<ulong> suffixes;

	superior_item_class_type superiority;

	ulong set_code;
	ulong unique_code;

	ulong runeword_id;
	ulong runeword_parameter;

	std::string personalised_name;

	bool is_armor;
	bool is_weapon;
	ulong defense;

	bool indestructible;
	ulong durability;
	ulong maximum_durability;

	ulong sockets;

	std::vector<item_property_type> properties;

	bool operator<(item_type const & other) const;
};

#endif
