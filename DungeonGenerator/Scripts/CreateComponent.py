import os

class_name = input('What component would you like to add?\n')
source_dir = '..\\Source\\Logic\\Components\\'

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
header_file_template = open('../Data/ComponentTemplates/TemplateComponentHeader.txt', 'r')
header_file_copy = open(clean_target_dir + '\\' + clean_class + '.h', 'w')
header_file_data = header_file_template.read()

# Copy data
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
source_file_template = open('../Data/ComponentTemplates/TemplateComponentCpp.txt', 'r')
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
