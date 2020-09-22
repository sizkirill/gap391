import os
import shutil

project_name = input('What project would you like to add?\n')
project_name = project_name.strip('\r')
os.makedirs(name = project_name + '/Assets', exist_ok = True)
os.makedirs(name = project_name + '/Builds', exist_ok = True)
os.makedirs(name = project_name + '/Source/Application', exist_ok = True)
os.makedirs(name = project_name + '/Source/Logic/Components', exist_ok = True)
os.makedirs(name = project_name + '/Source/Logic/Event', exist_ok = True)
os.makedirs(name = project_name + '/Source/Logic/Process', exist_ok = True)
os.makedirs(name = project_name + '/Source/Views', exist_ok = True)
os.makedirs(name = project_name + '/Scripts', exist_ok = True)
os.makedirs(name = project_name + '/Data/ComponentTemplates', exist_ok = True)
os.makedirs(name = project_name + '/Data/ClassTemplates', exist_ok = True)

another_name = input('Would you like to have another name for project, that is going to be shown in window? (y/n) \n').strip('\r')

if another_name.lower() == 'y':
	another_name = input('What name would you like? \n').strip('\r')
else:
	another_name = project_name

app_layer_name = input('Would you like a custom name for Application Layer? (y/n) \n').strip('\r')

if app_layer_name.lower() == 'y':
	app_layer_name = input('What name would you like? \n').strip('\r')
else:
	app_layer_name = project_name + '_AppLayer'

game_layer_name = input('Would you like a custom name for Application Layer? (y/n) \n').strip('\r')

if game_layer_name.lower() == 'y':
	game_layer_name = input('What name would you like? \n').strip('\r')
else:
	game_layer_name = project_name + '_GameLayer'

replace_dictionary = {'$app_layer':app_layer_name, '$game_layer':game_layer_name, '$project_name':another_name, '$main':'Main', '_source.txt':'.cpp', '_header.txt':'.h'}

for root, dirs, files in os.walk('Templates'):
    for dir in dirs:
        os.makedirs(name = project_name + '/Source/' + root.lstrip('Templates\\') + '/' + dir, exist_ok = True)
    for file in files:
        with open(os.path.join(root, file), 'r') as template:
            template_data = template.read()
            out_file = project_name + '/Source/' + root.lstrip('Templates\\') + '/' + file
            for k, v in replace_dictionary.items():
                template_data = template_data.replace(k, v)
                out_file = out_file.replace(k, v)
            print('Copying template to: ' + out_file)
            with open(out_file, 'w') as output:
                output.write(template_data)

for root, dirs, files in os.walk('ProjectData'):
    for dir in dirs:
        os.makedirs(name = (project_name + '/' + root.lstrip('ProjectData').lstrip('\\').lstrip('\\') + '/' + dir).replace('\\', '/'), exist_ok = True)
    for f in files:
        out_dir = (project_name + '/' + root.lstrip('ProjectData').lstrip('\\').lstrip('\\') + '/').replace('\\', '/')
        print('Copying ' + os.path.join(root,f) + ' to ' + out_dir)
        shutil.copy(os.path.join(root, f), out_dir)

print('Editing premake5 file...')
with open('premake5.lua', 'r') as premake5_file:
    premake5_file_data = premake5_file.read()
    if premake5_file_data.find("project \"{}\"".format(project_name)) != -1:
        quit(1)

with open('premake5_template.txt', 'r') as template:
    template_data = template.read()
    template_data = template_data.replace('$project_name', project_name)
    with open('premake5.lua', 'a') as premake5_file:
        premake5_file.write(template_data)

print('Generating Visual Studio project files...')
os.system("premake5 vs2019")

print('Done!')