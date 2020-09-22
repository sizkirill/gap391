import os
import shutil

source_project_name = input('What project would you like to copy?\n').strip('\r')
project_name = input('Enter the name of a new project?\n')
project_name = project_name.strip('\r')

os.makedirs(name=project_name, exist_ok=True)

print('Copying source files...')
shutil.copytree(src=source_project_name + '/Source/', dst=project_name + '/Source/')
print('Copying assets files...')
shutil.copytree(src=source_project_name + '/Assets/', dst=project_name + '/Assets/')
print('Copying scripts files...')
shutil.copytree(src=source_project_name + '/Scripts/', dst=project_name + '/Scripts/')
print('Copying data files...')
shutil.copytree(src=source_project_name + '/Data/', dst=project_name + '/Data/')

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