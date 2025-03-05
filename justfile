version := '2.0.1'

cc := 'gcc'
cflags := '-Wall -std=c23 -pedantic -W -Wall -Wextra -Werror'
output_dir := '../../'

output_file := 'dirwalk'

build:
    {{cc}} {{cflags}} -o {{output_dir}}{{output_file}} lab1.c

clean:
    rm -f {{output_dir}}{{output_file}}

run:    
    {{output_dir}}{{output_file}}
