import os,sys

ATTRIBUTES = ['max_health', 'health_regeneration',
    'dmg_physical', 'dmg_fire', 'dmg_ice', 'dmg_light', 'dmg_shadow',
    'res_physical', 'res_fire', 'res_ice', 'res_light', 'res_shadow',
    'haste', 'critical', 'heal']
    
ATTRIBUTES_W = ATTRIBUTES + ['chop_damage']
   
def replace_equipment():
    with open('balancing.CSV') as f:
        content = f.readlines()
        
    #strip the first two lines
    content = content[2:]

    attr_dict = {}
    
    for line in content:
        attributes = line.split(';')
        
        if attributes[1]!='eq_weapon':
            attr_list = []
            for i in range(2, 2 + len(ATTRIBUTES)):
                attr_list = attr_list + [int(attributes[i])]
            
            attr_dict[attributes[0]] = attr_list
            
    with open('../insert_scripts/item/insert_items_equipment_utf8.sql') as f:
        content = f.readlines()
        f.close()
    
    for i in range(len(content)):
        if 'item_attribute' not in content[i]:
            continue
        
        item_id = (content[i].split("values ('")[1]).split("'")[0]
        
        attr_list = attr_dict[item_id]
        local_attr_dict = {}
        
        for a in range(len(ATTRIBUTES)):
            if (attr_list[a] != 0):
                local_attr_dict[ATTRIBUTES[a]] = attr_list[a]
                
        key_string = ""
        val_string = ""
        
        for key, val in local_attr_dict.iteritems():
            key_string = key_string + ", " + key
            val_string = val_string + ", " + str(val)
        
        new_string = "INSERT INTO item_attribute (item_id"
        new_string = new_string + key_string
        new_string = new_string + ") values ('" + item_id + "'"
        new_string = new_string + val_string
        new_string = new_string + ");\n"
        
        content[i] = new_string
        
    # now, write it down
    with open('../insert_scripts/item/insert_items_equipment_utf8.sql', 'w') as file:
        for line in content:
            file.write(line)
            
        file.close()
    
def replace_weapon():
    with open('balancing.CSV') as f:
        content = f.readlines()
        
    #strip the first two lines
    content = content[2:]

    attr_dict = {}
    
    for line in content:
        attributes = line.split(';')
        
        if attributes[1]=='eq_weapon':
            attr_list = []
            for i in range(2, 2 + len(ATTRIBUTES_W)):
                attr_list = attr_list + [int(attributes[i])]
            
            attr_dict[attributes[0]] = attr_list
            
    with open('../insert_scripts/item/insert_items_weapon_utf8.sql') as f:
        content = f.readlines()
        f.close()
    
    for i in range(len(content)):
        if ('item_attribute' not in content[i]) and ('item_weapon ' not in content[i]):
            continue
        
        item_id = (content[i].split("values ('")[1]).split("'")[0]
        
        
        if ('item_attribute' in content[i]):
        
            attr_list = attr_dict[item_id]
            local_attr_dict = {}
            
            for a in range(len(ATTRIBUTES)):
                if (attr_list[a] != 0):
                    local_attr_dict[ATTRIBUTES[a]] = attr_list[a]
                    
            key_string = ""
            val_string = ""
            
            for key, val in local_attr_dict.iteritems():
                key_string = key_string + ", " + key
                val_string = val_string + ", " + str(val)
            
            new_string = "INSERT INTO item_attribute (item_id"
            new_string = new_string + key_string
            new_string = new_string + ") values ('" + item_id + "'"
            new_string = new_string + val_string
            new_string = new_string + ");\n"
            
            content[i] = new_string
            
        else:
            
            new_string = content[i].rsplit(',', 1)[0]
            new_string = new_string + ', ' + str(attr_list[-1]) + ');\n'
            content[i] = new_string
        
    # now, write it down
    with open('../insert_scripts/item/insert_items_weapon_utf8.sql', 'w') as file:
        for line in content:
            file.write(line)
            
        file.close()

def main(): 
    replace_equipment()
    replace_weapon()

    return 1

if __name__ == "__main__":
    sys.exit(main())
