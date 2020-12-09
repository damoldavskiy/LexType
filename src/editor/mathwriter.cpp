#include "mathwriter.h"

#include "styler.h"
#include "keyboardlayout.h"
#include "interval.h"

QVector<QChar> delimeters = { ' ', ',', ':', ';', '\n', '\t' };
QVector<QChar> openBrackets = { '(', '{', '[' };
QVector<QChar> closeBrackets = { ')', '}', ']' };

bool isDelimeter(QChar symbol)
{
    return delimeters.contains(symbol);
}

bool isOpenBracket(QChar symbol, int direction)
{
    if (direction == 1)
        return openBrackets.contains(symbol);
    else
        return closeBrackets.contains(symbol);
}

bool isCloseBracket(QChar symbol, int direction)
{
    if (direction == 1)
        return closeBrackets.contains(symbol);
    else
        return openBrackets.contains(symbol);
}

QChar getClosing(QChar open, int direction)
{
    if (direction == 1)
        return closeBrackets[openBrackets.indexOf(open)];
    else
        return openBrackets[closeBrackets.indexOf(open)];
}

bool isClosing(QChar open, QChar close, int direction)
{
    if (direction == 1)
        return openBrackets.indexOf(open) != -1 && openBrackets.indexOf(open) == closeBrackets.indexOf(close);
    else
        return openBrackets.indexOf(close) != -1 && openBrackets.indexOf(close) == closeBrackets.indexOf(open);
}

QString MathWriter::pass(const QString &source)
{
    QString result;
    QString math;

    Interval interval;
    QChar last = '\0';
    bool display = false;

    QVector<QPair<QString, QString>> dict = snippetList();

    for (int i = 0; i < source.size(); ++i) {
        interval.update(last, source[i]);

        if (interval.type() != IntervalType::Mathematics || i == source.size() - 1) {
            if (math.size() > 0) {
                if (display && math.size() > 3)
                    result += "$$" + apply(math.mid(2, math.size() - 4), dict) + "$$";
                else if (math.size() > 1)
                    result += "$" + apply(math.mid(1, math.size() - 2), dict) + "$";
                math = "";
            }
        }

        if (interval.type() != IntervalType::Mathematics) {
            if (source[i] == '`') // Escaped `
                result.back() = source[i];
            else
                result += source[i];
        } else {
            if (math.isEmpty())
                display = false;
            else if (math.size() == 1 && source[i] == '`')
                display = true;

            if (source[i] == '`' && interval.isEscape())
                math.back() = source[i];
            else
                math += source[i];
        }

        last = source[i];
    }

    return result;
}

QVector<QPair<QString, QString>> MathWriter::snippetList()
{
    QVector<QPair<QString, QString>> dict;

    dict.append({ "(", "\\left(" });
    dict.append({ ")", "\\right)" });
    dict.append({ "[", "\\left[" });
    dict.append({ "]", "\\right]" });
    dict.append({ "⟨", "\\left\\langle" });
    dict.append({ "⟩", "\\right\\rangle" });

    dict.append({ "...", "\\dots" });
    dict.append({ "⋅⋅⋅", "\\mdots" });
    dict.append({ "⋯", "\\cdots" });
    dict.append({ "⋮", "\\vdots" });
    dict.append({ "⋱", "\\ddots" });

    dict.append({ "×", "\\times" });
    dict.append({ "⋅", "\\cdot" });
    dict.append({ "∘", "\\circ" });
    dict.append({ "⊕", "\\oplus" });
    dict.append({ "⊗", "\\otimes" });

    dict.append({ "sin", "\\sin" });
    dict.append({ "cos", "\\cos" });
    dict.append({ "sqrt", "\\sqrt" });
    dict.append({ "exp", "\\exp" });
    dict.append({ "ker", "\\ker" });

    dict.append({ "text", "\\text" });
    dict.append({ "comment", "\\comment" });

    dict.append({ "lim", "\\lim" });
    dict.append({ "∑", "\\sum" });
    dict.append({ "∏", "\\prod" });
    dict.append({ "∫", "\\int" });

    dict.append({ "oline", "\\overline" });
    dict.append({ "uline", "\\underline" });

    dict.append({ "⇔", "\\iff" });
    dict.append({ "↔", "\\leftrightarrow" });
    dict.append({ "⇒", "\\implies" });
    dict.append({ "⇐", "\\impliedby" });
    dict.append({ "→", "\\to" });
    dict.append({ "←", "\\leftarrow" });

    dict.append({ "∀", "\\forall" });
    dict.append({ "∈", "\\in" });
    dict.append({ "∃", "\\exists" });
    dict.append({ "∩", "\\cap" });
    dict.append({ "∪", "\\cup" });
    dict.append({ "⊆", "\\subseteq" });
    dict.append({ "⊂", "\\subset" });
    dict.append({ "⊇", "\\supseteq" });
    dict.append({ "⊃", "\\supset" });
    dict.append({ "not", "\\not" });

    dict.append({ "≠", "\\ne" });
    dict.append({ "±", "\\pm" });
    dict.append({ "≈", "\\approx" });
    dict.append({ "≡", "\\equiv" });
    dict.append({ "≅", "\\cong" });

    dict.append({ "≤", "\\le" });
    dict.append({ "≥", "\\ge" });
    dict.append({ "≪", "\\ll" });
    dict.append({ "≫", "\\gg" });

    dict.append({ "ℕ", "\\mathbb{N}" });
    dict.append({ "ℤ", "\\mathbb{Z}" });
    dict.append({ "ℚ", "\\mathbb{Q}" });
    dict.append({ "ℝ", "\\mathbb{R}" });
    dict.append({ "ℂ", "\\mathbb{C}" });
    dict.append({ "ℙ", "\\mathbb{P}" });

    dict.append({ "∅", "{\\varnothing}" });
    dict.append({ "∞", "{\\infty}" });
    dict.append({ "∂", "{\\partial}" });

    dict.append({ "bar", "\\bar" });
    dict.append({ "hat", "\\hat" });
    dict.append({ "vec", "\\vec" });

    dict.append({ "α", "{\\alpha}" });
    dict.append({ "β", "{\\beta}" });
    dict.append({ "Γ", "{\\Gamma}" });
    dict.append({ "γ", "{\\gamma}" });
    dict.append({ "Δ", "{\\Delta}" });
    dict.append({ "δ", "{\\delta}" });
    dict.append({ "ɛ", "{\\epsilon}" }); // varepsilon
    dict.append({ "ζ", "{\\zeta}" });
    dict.append({ "η", "{\\eta}" });
    dict.append({ "Θ", "{\\Theta}" });
    dict.append({ "ϑ", "{\\theta}" }); // vartheta
    dict.append({ "ι", "{\\iota}" });
    dict.append({ "κ", "{\\kappa}" });
    dict.append({ "Λ", "{\\Lambda}" });
    dict.append({ "λ", "{\\lambda}" });
    dict.append({ "μ", "{\\mu}" });
    dict.append({ "ν", "{\\nu}" });
    dict.append({ "Ξ", "{\\Xi}" });
    dict.append({ "ξ", "{\\xi}" });
    dict.append({ "Π", "{\\Pi}" });
    dict.append({ "π", "{\\pi}" });
    dict.append({ "ρ", "{\\rho}" });
    dict.append({ "Σ", "{\\Sigma}" });
    dict.append({ "σ", "{\\sigma}" });
    dict.append({ "τ", "{\\tau}" });
    dict.append({ "υ", "{\\upsilon}" });
    dict.append({ "Φ", "{\\Phi}" });
    dict.append({ "φ", "{\\phi}" }); // varphi
    dict.append({ "χ", "{\\chi}" });
    dict.append({ "Ψ", "{\\Psi}" });
    dict.append({ "ψ", "{\\psi}" });
    dict.append({ "Ω", "{\\Omega}" });
    dict.append({ "ω", "{\\omega}" });

    return dict;
}

QString MathWriter::applyParameters(QString source)
{
    for (int i = 0; i < source.size() - 1; ++i)
        if (source[i] == '_' || source[i] == '^') {
            int end = findBracketPlace(source, i, 1, { '_', '^', '/' });
            if (source[i + 1] == '(' && source[end] == ')') {
                source.remove(end, 1);
                source.remove(i + 1, 1);
                end -= 2;
            }
            if (source[i + 1] != '{') {
                source.insert(end + 1, '}');
                source.insert(i + 1, '{');
            }
        }

    return source;
}

QString MathWriter::applyMatrices(QString source)
{
    if (!Styler::get<bool>("math-flag-matrices"))
        return source;

    QVector<QVector<QString>> matrices {
        { "\\(", ")", "pmatrix" },
        { "\\[", "]", "bmatrix" },
        { "\\{", "}", "Bmatrix" },
        { "{{", "}}", "cases" }
    };

    for (int i = 0; i < source.size() - 1; ++i)
        for (const QVector<QString> &pattern : matrices)
            if (source.size() - i >= pattern[0].size() && source.mid(i, pattern[0].size()) == pattern[0]) {

                int start = i;

                QStack<QChar> brackets;
                for (++i; i < source.size(); ++i) {
                    if (isOpenBracket(source[i]))
                        brackets.push(source[i]);
                    else if (isCloseBracket(source[i])) {
                        if (brackets.size() > 0 && isClosing(brackets.top(), source[i]))
                            brackets.pop();
                        else
                            break;
                    } else {
                        if (brackets.size() == 1) {
                            if (source[i] == ',') {
                                if (pattern[2] != "cases") {
                                    source.replace(i, 1, " &");
                                    i += 2;
                                } else {
                                    source.replace(i, 1, " ,&");
                                    i += 3;
                                }
                            } else if (source[i] == ';') {
                                source.replace(i, 1, " \\\\");
                                i += 3;
                            }

                        }
                    }

                    if (brackets.size() == 0)
                        break;
                }

                source.replace(i, pattern[1].size(), " \\end{" + pattern[2] + "}");
                source.replace(start, pattern[0].size(), "\\begin{" + pattern[2] + "} ");

                i = start + 6 + pattern[2].size();
            }

    return source;
}

QString MathWriter::applyFractions(QString source)
{
    if (!Styler::get<bool>("math-flag-fractions"))
        return source;

    for (int i = 0; i < source.size(); ++i) {
        if (source[i] == '/') {
            int leftBrace = findBracketPlace(source, i, -1);
            int rightBrace = findBracketPlace(source, i);

            if (leftBrace != i && rightBrace != i) {
                if (source[leftBrace] == '-')
                    ++leftBrace;
                if (source[i - 1] == ')' && source[leftBrace] == '(') {
                    source.remove(i - 1, 1);
                    source.remove(leftBrace, 1);
                    i -= 2;
                    rightBrace -= 2;
                }
                if (source[i + 1] == '(' && source[rightBrace] == ')') {
                    source.remove(rightBrace, 1);
                    source.remove(i + 1, 1);
                    rightBrace -= 2;
                }
                source.remove(i, 1);
                source.insert(rightBrace, "}");
                source.insert(i, "}{");
                source.insert(leftBrace, "\\frac{");
            }
        }
    }

    return source;
}

QString MathWriter::applySnippets(QString source, const QVector<QPair<QString, QString>> &dict)
{
    QString result;
    for (int i = 0; i < source.size(); ++i) {
        bool next = false;
        for (const auto &pair : dict)
            if (i + pair.first.size() <= source.size() && source.mid(i, pair.first.size()) == pair.first) {
                result += pair.second;
                i += pair.first.size() - 1;
                next = true;
                break;
            }
        if (next)
            continue;

        if (source[i] == ':' && i > 0 && !isDelimeter(source[i - 1])) {
            result += " \\colon";
            continue;
        }

        result += source[i];
    }

    return result;
}

QString MathWriter::applyCyrillic(QString source)
{
    for (int i = 0; i < source.size(); ++i)
        if (KeyboardLayout::isCyrillicSpecific(source[i])) {
            if (i > 0 && source[i - 1] == '{') {
                int end = findBracket(source, i - 1);
                if (end != -1) {
                    source.insert(i - 1, "\\text");
                    i = end + 5;
                }
            } else {
                source.insert(i, "\\text{");
                int end = findBracketPlace(source, i + 5);
                source.insert(end + 1, '}');
                i = end;
            }
        }

    return source;
}

QString MathWriter::apply(QString source, const QVector<QPair<QString, QString>> &dict)
{
    // TODO More effective
    source = applyParameters(source);
    source = applyMatrices(source);
    source = applyFractions(source);
    source = applySnippets(source, dict);
    source = applyCyrillic(source);

    return source;
}

