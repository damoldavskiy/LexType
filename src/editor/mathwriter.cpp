#include "mathwriter.h"

QVector<QChar> delimeters = { ' ', ',', ':', ';', '\n', '\t' };
QVector<QChar> openBraces = { '(', '{', '[' };
QVector<QChar> closeBraces = { ')', '}', ']' };

bool isDelimeter(QChar symbol)
{
    return delimeters.contains(symbol);
}

bool isOpenBrace(QChar symbol, int direction)
{
    if (direction == 1)
        return openBraces.contains(symbol);
    else
        return closeBraces.contains(symbol);
}

bool isCloseBrace(QChar symbol, int direction)
{
    if (direction == 1)
        return closeBraces.contains(symbol);
    else
        return openBraces.contains(symbol);
}

bool isClosing(QChar open, QChar close, int direction)
{
    if (direction == 1)
        return openBraces.indexOf(open) != -1 && openBraces.indexOf(open) == closeBraces.indexOf(close);
    else
        return openBraces.indexOf(close) != -1 && openBraces.indexOf(close) == closeBraces.indexOf(open);
}

QString MathWriter::pass(const QString &source)
{
    QString result;
    bool math = false;
    int start;

    for (int i = 0; i < source.size(); ++i) {
        if (source[i] == '`') {
            math = !math;
            if (math)
                start = i + 1;
            else // TODO string_view
                result += "$" + apply(source.mid(start, i - start)) + "$";
        } else if (!math) {
            result += source[i];
        }
    }

    return result;
}

QString MathWriter::applyParameters(QString source)
{
    // TODO \\sqrt(x + 2) -> \\sqrt{x + 2}, \\lim(x -> 0) -> \\lim_{x -> 0}
    return source;
}

QString MathWriter::applyMatrices(QString source)
{
    QVector<QVector<QString>> matrices {
        { "\\\\left(", "\\right)", "pmatrix" },
        { "\\\\left[", "\\right]", "bmatrix" },
        { "\\{", "}", "Bmatrix" },
        { "\\|", "|", "vmatrix" },
    };

    for (int i = 0; i < source.size() - 1; ++i)
        for (const QVector<QString> &pattern : matrices)
            if (source.size() - i >= pattern[0].size() && source.mid(i, pattern[0].size()) == pattern[0]) {
                source.replace(i, pattern[0].size(), "\\begin{" + pattern[2] + '}');
                i += 8 + pattern[2].size();
                for (; source.size() - i >= pattern[1].size() && source.mid(i, pattern[1].size()) != pattern[1]; ++i) {
                    if (source[i] == ',') {
                        source.replace(i, 1, " & ");
                        i += 3;
                    } else if (source[i] == ';') {
                        source.replace(i, 1, " \\\\ ");
                        i += 4;
                    }
                }
                source.replace(i, pattern[1].size(), "\\end{" + pattern[2] + "}");
                i += 6 + pattern[2].size();
                break;
            }

    return source;
}

QString MathWriter::applyFractions(QString source)
{
    for (int i = 0; i < source.size(); ++i) {
        if (source[i] == '/') {
            int leftBrace = findBracePlace(source, i, -1);
            int rightBrace = findBracePlace(source, i, 1);

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

QString MathWriter::apply(const QString &source)
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

    dict.append({ "lim", "\\lim" });
    dict.append({ "∑", "\\sum" });
    dict.append({ "∏", "\\prod" });
    dict.append({ "∫", "\\int" });

    dict.append({ "oline", "\\overline" });
    dict.append({ "uline", "\\underline" });

    dict.append({ "matrix", "\\matrix" });
    dict.append({ "system", "\\system" });

    dict.append({ "⇔", "\\iff" });
    dict.append({ "↔", "\\leftrightarrow" });
    dict.append({ "⇒", "\\implies" });
    dict.append({ "⇐", "\\impliedby" });
    dict.append({ "→", "\\to" });
    dict.append({ "←", "\\leftarrow" });

    dict.append({ "∀", "\\forall" });
    dict.append({ "∈", "\\in" });
    dict.append({ "∃", "\\exists" });
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
    dict.append({ "Δ", "{Delta}" });
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

    // TODO More effective
    result = applyParameters(result);
    result = applyMatrices(result);
    result = applyFractions(result);

    return result;
}

