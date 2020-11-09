import os
import re
import sys
import subprocess


SCRIPT_DIRECTORY = os.path.dirname(os.path.realpath(__file__))
COMPILER_PATH = os.path.join(SCRIPT_DIRECTORY, '..', 'build', 'source', 'main', 'Debug', 'CringeLang.exe')

cases = [
    {
        'directory': f'{SCRIPT_DIRECTORY}/parsing/',
        'command': COMPILER_PATH + ' --std 1',
    },
]


def pad_contents(text, padding='> '):
    return padding + text.replace('\n', '\n' + padding)


def remove_links(text):
    # links contain local-pc full paths to test files
    text = re.sub(r'Quick Link >[^\n]*\n', '', text)
    text = re.sub(r'\*\*\* FILE [^\n]*\n', '', text)
    return text


def test(case, input_file):
    command = case['command'].split()
    command.append(os.path.join(case["directory"], input_file))
    actual = subprocess.run(command, capture_output=True, text=True).stdout
    actual = remove_links(actual)

    base_name = os.path.splitext(input_file)[0]
    output_file = base_name + '.out'
    desired = None

    with open(os.path.join(case['directory'], output_file), 'r', encoding='utf-8') as file:
        desired = remove_links(file.read())

    if len(sys.argv) >= 2 and sys.argv[1] == 'apply':
        with open(os.path.join(case['directory'], output_file), 'w', encoding='utf-8') as file:
            file.write(actual)

        print(f'[updated] {base_name}')
        return True

    if actual != desired:
        print(f'[bad] {base_name}')
        print("")
        print(pad_contents(desired, ' - |'))
        print("")
        print(pad_contents(actual, ' + |'))
        print("")
    else:
        print(f'[good] {base_name}')

    return actual == desired


def run(case):
    number_of_good = 0
    total = 0

    for file in os.listdir(case['directory']):
        if file.endswith('.in'):
            total += 1
            is_good = test(case, file)

            if is_good:
                number_of_good += 1

    print(f'Passed: {number_of_good}, failed: {total - number_of_good}, total: {total}')


if __name__ == '__main__':
    for case in cases:
        run(case)
