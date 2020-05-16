import sys
from os import walk
from os.path import join

line_size = 70

def write_section(section, section_path):
    result = '\section{' + section + '}\n'
    
    for (path, dirs, files) in walk(section_path):
        for file in files:
            print(file)
            result += '\n\\subsection{' + file.replace('_', '\\_') + '}\n\\begin{verbatim}\n'
            with open(join(path, file)) as fin:
                for line in fin.readlines():
                    line = line[:-1]
                    while len(line) > 0:
                        result += line[:line_size].encode('utf-8').decode('ascii', 'backslashreplace') + '\n'
                        line = line[line_size:]
            result += '\\end{verbatim}\n'
    result += '\n'
    return result

result = r'''\documentclass[listing]{espd}
\usepackage[russian]{babel}

\bibliographystyle{gost2008}

\managerrank{Научный руководитель,\\доцент департамента\\программной инженерии\\факультета компьютерных наук,\\канд. техн. наук}
\manager{С.Л. Макаров}

\authorrank{студент группы БПИ183}
\author{Д.А. Молдавский}

\title{Программная среда для записи\\математических лекций}
\code{04.03}

\city{Москва}
\year{2020}

\begin{document}

\annotation

Текст программы -- это документ, содержащий исходный код программы. Каждому файлу соответствует его подраздел. Ввиду того, что в исходном коде встречаются символы Unicode, данные символы в приведенном ниже коде заменены на обозначения через обратную косую черту. В данном документе содержатся следующие разделы: <<Исходный код редактора>>, <<Исходный код просмотрщика>>, <<Исходный код тестов>>, <<Исходный код бенчмарков>>.

В разделе <<Исходный код редактора>> приведен код редактора текста и изображений.

В разделе <<Исходный код просмотрщика>> приведен код просмотрщика PDF документов, основанного на библиотеке Poppler.

В разделе <<Исходный код тестов>> приведен код программы тестов.

В разделе <<Исходный код бенчмарков>> приведен код бенчмарков.

Настоящий Текст программы удовлетворяет требованиям ГОСТ 19.401-78~\cite{espd401}.

\tableofcontents
'''

result += write_section('Исходный код редактора', '../src/editor')
result += write_section('Исходный код просмотрщика', '../src/viewer')
result += write_section('Исходный код тестов', '../tests/auto')
result += write_section('Исходный код бенчмарков', '../tests/benchmarks')

result += r'''
\bibliography{espd,library}

\end{document}
'''

with open('listing.tex', 'w') as fout:
    fout.write(result)
