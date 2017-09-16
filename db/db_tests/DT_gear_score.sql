SELECT i.item_id, i.item_type, a.max_health, a.health_regeneration,
    a.dmg_physical, a.dmg_fire, a.dmg_ice, a.dmg_light, a.dmg_shadow,
    a.res_physical, a.res_fire, a.res_ice, a.res_light, a.res_shadow,
    a.haste, a.critical, a.heal, 0 FROM
    item AS i JOIN item_attribute AS a ON a.item_id=i.item_id
    WHERE i.item_type<>'eq_weapon' AND i.item_type<>'consumable' AND i.item_type<>'permanent';
    
SELECT i.item_id, i.item_type, a.max_health, a.health_regeneration,
    a.dmg_physical, a.dmg_fire, a.dmg_ice, a.dmg_light, a.dmg_shadow,
    a.res_physical, a.res_fire, a.res_ice, a.res_light, a.res_shadow,
    a.haste, a.critical, a.heal, w.chop_damage FROM item AS i JOIN item_attribute AS a ON a.item_id=i.item_id
    JOIN item_weapon AS w ON w.item_id=i.item_id;