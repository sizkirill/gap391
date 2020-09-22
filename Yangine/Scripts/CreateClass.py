import os

class_name = input('What class would you like to add?\n')
is_virtual = input('Is it a base class? (y/n) \n').strip('\r')
has_parent = input('Does it inherit from a class? (y/n)\n').strip('\r')
parent_name = ''
parent_dir = ''
source_dir = '..\\Source\\'

if has_parent.lower() == 'y':
    parent_name = input('What class does it inherit from?\n').strip('\r')
    found_parent = False

    for root, dirs, files in os.walk(source_dir):
        for name in files:
            if name == parent_name + '.h':
                found_parent = True
                has_parent = True
                parent_dir = root
                print('Found {0} in {1}'.format(parent_name, parent_dir))
                break

    if not found_parent:
        print('Could not find class {0}\n'.format(parent_name))
        quit(1)
else:
    has_parent = False

if is_virtual.lower() == 'y':
	is_virtual = True
else:
	is_virtual = False

target_dir = input('Which directory should it live in?\n')

target_dir = source_dir + target_dir
clean_target_dir = target_dir.strip('\r')
clean_class = class_name.strip('\r')
create_class = True

dir_exists = os.path.isdir(clean_target_dir)

if not dir_exists:
    print('{0} does not exist\n'.format(clean_target_dir))
    make_dir = input('Would you like to make it?  (y/n)\n').strip('\r')

    if make_dir.lower() == 'y':
        os.mkdir(clean_target_dir)
    else:
        print('Exiting...')
        quit(1)

# -- COPY HEADER --

# Open files
header_file_template = open('../Data/ClassTemplates/TemplateClassHeader.txt', 'r')
header_file_copy = open(clean_target_dir + '\\' + clean_class + '.h', 'w')
header_file_data = ''

# Copy data
if has_parent:
    # Calculate relative path
    path_to_parent = os.path.relpath(parent_dir, clean_target_dir)
    header_file_data = header_file_template.read()

    # Replace identifiers
    header_file_data = header_file_data.replace('$parent_name', parent_name)
    header_file_data = header_file_data.replace('$path_to_parent', path_to_parent + '\\' + parent_name)

else:
    header_file_lines = header_file_template.readlines()

    for line in header_file_lines:
        if '$parent_name' not in line and '$path_to_parent' not in line:
            header_file_data += line
			
if is_virtual:
    header_file_data = header_file_data.replace('$is_virtual', 'virtual ')
else:
    header_file_data = header_file_data.replace('$is_virtual', '')	

header_file_data = header_file_data.replace('$class_name', clean_class)
header_file_data = header_file_data.replace('$file_name', clean_class)

# Write data
header_file_copy.write(header_file_data)

# Close files
header_file_copy.close()
header_file_template.close()

print('Copied template to ' + clean_class + ".h")
# -- COPY SOURCE --

# Open files
source_file_template = open('../Data/ClassTemplates/TemplateClassCpp.txt', 'r')
source_file_copy = open(clean_target_dir + '\\' + clean_class + '.cpp', 'w')

# Copy data
source_file_data = source_file_template.read()
source_file_data = source_file_data.replace('$class_name', clean_class)

# Write data
source_file_copy.write(source_file_data)

# Close files
source_file_copy.close()
source_file_template.close()
print('Copied template to ' + clean_class + ".cpp")
