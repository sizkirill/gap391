import os

ignore_list = ['tinyxml2.h', 'tinyxml2.cpp']

file_count = 0
empty_line_count = 0
line_count = 0
comment_count = 0

def parse_directory(path):
    for root, dirs, files in os.walk(path):
        for file in files:
            if file not in ignore_list:
                global file_count
                file_count += 1
                parse_file(os.path.join(root, file))

def parse_file(path):
    global empty_line_count
    global line_count
    global comment_count

    # Used for detecting block comments
    in_block_comment = False

    file = open(path)
    lines = file.readlines()
    for line in lines:
        line = line.strip('\t \n')
        if len(line) == 0:
            empty_line_count += 1
            continue

        if in_block_comment:
            comment_count += 1
            if '*/' in line:
                in_block_comment = False
            continue

        if '/*' in line:
            if '*/' in line:
                comment_count += 1
            else:
                in_block_comment = True
            continue

        if '//' in line:
            # There is a line comment somewhere
            if line[0] == '/' and line[1] == '/':
                comment_count += 1
                continue

            # There is a comment after a line of code
            comment_count += 1
            line_count +=1
            continue

        line_count += 1


parse_directory('Yangine/Source')
parse_directory('GAP321_KirillSizykh/Source')

print('Files Found: ' + str(file_count))
print('Line Count: ' + str(line_count))
print('Comment Count: ' + str(comment_count))
print('Empty Line Count: ' + str(empty_line_count))