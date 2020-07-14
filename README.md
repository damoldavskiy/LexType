# LexType

Программа предназначена для записи лекций по математическим предметам на языке, близком к LaTeX. Благодаря множеству специальных функций конспектирование на русском языке с использованием большого количества формул значительно облегчено. Программа включает редактор с подсветкой синтаксиса и другими возможностями, графический векторный редактор и подпрограмму для просмотра PDF файлов.

## Установка
Чтобы скачать последнюю версию приложения, нужно найти архив для целевой операционной системе на [странице релизов](https://github.com/damoldavskiy/LexType/releases). Приложение можно запустить сразу, но для коррктной работы необходимо проделать дополнительные действия, в зависимости от системы.

### Для пользователей Linux
Необходимо убедиться, что установлены пакеты `texlive-latex-base`, `texlive-latex-extra`, `texlive-lang-cyrillic`. Они нужны для работы компилятора LaTeX и корректной работы с русским языком. Также нужны библиотеки `libpoppler-qt5-1`, `libquazip5-1`, которые являются зависимостями подпрограмм LexType.

Для использования новых макрокоманд необходимо переместить стилевой файл `lexpack.sty` в директорию `~/texmf/tex/latex/lexpack/`.

Для запуска подпрограмм необходимо из консоли выполнить команду выполнения файла, например, `./editor`.

### Для пользователей Windows
Необходимо установить TeX Live или MiKTeX (по желанию). Нужно убедиться, что папка с исполняемым файлом pdflatex.exe содержится в переменной окружения PATH. Для проверки достаточно в командной строке (cmd.exe) выполнить команду `pdflatex --version`. Остальные библиотеки из зависимостей уже включены в архив.

Для использования новых макрокоманд необходимо переместить стилевой файл `lexpack.sty` в ту директорию, в которой будут содержаться компилируемые файлы. Опционально, можно хранить его в директории `C:\Users\<user name>\texmf\tex\latex\lexpack\` (при установленном дистрибутиве TeX Live) или `C:\Users\<user name>\AppData\Local\MiKTeX\<number>\tex\latex\lexpack\` (при установленном дистрибутиве `MiKTeX`).

## Обзор
В архиве содержится три программы:

* editor — редактор, основная программа приложения;
* viewer — программа для просмотра PDF;
* updater — программа для обновлений.

Также в архиве содержится файл version, который используется для обновлений и содержит версию установленной программы. В выпуске для Windows также содержатся все необходимые библиотеки.

## Обновления
Обновления программы выпускаются в виде релизов GitHub. При запуске редактора наличие обновлений не проверяется. Для запуска проверки необходимо закрыть editor и viewer, после чего запустить updater. При наличии новых релизов соответствующий архив для текущей ОС будет скачен, после чего программа будет обновлена.

## Текстовый редактор
Открывается при запуске основной программы editor. Редактор обладает подсеткой синтаксиса LaTeX, скобок, а также поддержкой сниппетов. Посмотреть их список можно в настройках, во вкладке Snippets. Флаг Math в настройках означает, что данный сниппет применяется в новом математическом режиме.

Примеры сниппетов:

* doc\<tab\> — вставляет пустой шаблон документа;
* sec\<tab\> — вставляет новый раздел;
* => — вставляет символ следования ⇒ в математическом режиме;
* lam — вставляет строчную букву λ в математическом режиме.

Перед началом использования настоятельно рекомендуется ознакомиться с полным списком сниппетов, т.к. без их знания использование нового математического режима, фактически, становится невозможным.

## Новый математический режим
Из-за того, что математический режим, используемый в LaTeX, недостаточно удобен, в программе используется новый математический режим. Его начало и окончание обозначается символом обратного апострофа "`". По умолчанию редактор воспринимает букву "ё" как начало нового математического режима, и раскладка внутри формулы меняется на противоположную (это связано с тем, что формула не может начинаться с русских букв).

Основные функции нового математического режима:

* Преобразование Unicode символов в LaTeX обозначения. Так, символ λ, получаемый с помощью сниппета, преобразуется в \lambda. Доступные символы определяются начальным набором сниппетов редактора;
* Преобразование некоторых распространенных команд. Так, `text` преобразуется в `\text`, `oline` в `\overline`, `uline` в `\underline`, `not` в `\not`. Аналогично `bar`, `hat`, `vec`, `sin`, `cos`, `sqrt`, `exp`.
* Запись деления в удобном формате. `1/2` преобразуется в `\frac{1}{2}`, а `(1 + 2)/sin(x + 1)` преобразуется в `\frac{1 + 2}{sin(x + 1)}`;
* Запись матриц и систем в удобном формате. `\(1, 2; 3, 4)` преобразуется в матрицу 2x2, в которой в первой строке записаны 1 и 2, во второй — 3 и 4. Для записи систем необходимо писать `{{x; y}}`.

## Графический редактор
При конспектировании регулярно необходимо вносить разного рода схемы и рисунки. Для открытия графического редактора следует использовать сочетание клавиш Ctrl+D. Для закрытия редактора с сохранением результата — снова Ctrl+D, для закрытия с удалением результата — Ctrl+Shift+D или крестик в заголовке окна. При вставке изображения в текст используется синтаксис TikZ.

При рисовании большинства фигур возможно использование модификатора (клавиша Shift). Так, прямоугольники становятся квадратами, а прямые прижимаются к углам, кратным 15 градусам.

Вставка текста отличается от реализации в большинстве графических редакторов. Чтобы вставить в изображение текст, необходимо выбрать его в панели инструментов (или нажать букву T), написать текст в мини-редакторе в нижней части окна, после чего кликнуть на место вставки. Для выхода из режима вставки текста необходимо выбрать любой другой инструмент или нажать Enter.

## Программа просмотра PDF
Программа просмотра PDF использует настройки основного приложения. Для их применения нужно перезапустить viewer. При обновлении файла PDF он будет перезагружен. Программы editor и viewer работают независимо, возможно использование любого другого приложения для просмотра PDF.

