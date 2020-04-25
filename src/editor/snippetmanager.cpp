#include "snippetmanager.h"

SnippetManager::SnippetManager()
{
    _regular.append(Snippet("sssec", "\\subsubsection{}", 15, true));
    _regular.append(Snippet("ssec", "\\subsection{}", 12, true));
    _regular.append(Snippet("sec", "\\section{}", 9, true));

    _regular.append(Snippet("section*{", "section{", true));
    _regular.append(Snippet("section{", "section*{", true));

    _regular.append(Snippet("thm", "\\begin{theorem}\n\t\n\\end{theorem}\n", 17, true));
    _regular.append(Snippet("def", "\\begin{definition}\n\t\n\\end{definition}\n", 20, true));
    _regular.append(Snippet("prf", "\\begin{proof}\n\t\n\\end{proof}\n", 15, true));
    _regular.append(Snippet("cor", "\\begin{corollary}\n\t\n\\end{corollary}\n", 19, true));
    _regular.append(Snippet("rmk", "\\begin{remark}\n\t\n\\end{remark}\n", 16, true));
    _regular.append(Snippet("lem", "\\begin{lemma}\n\t\n\\end{lemma}\n", 15, true));
    _regular.append(Snippet("exm", "\\begin{example}\n\t\n\\end{example}\n", 17, true));

    _regular.append(Snippet("\\begin{theorem}\n\t","\\begin{theorem}[]\n\t", 16, true));
    _regular.append(Snippet("\\begin{definition}\n\t", "\\begin{definition}[]\n\t", 19, true));
    _regular.append(Snippet("\\begin{proof}\n\t", "\\begin{proof}[]\n\t", 14, true));
    _regular.append(Snippet("\\begin{corollary}\n\t", "\\begin{corollary}[]\n\t", 18, true));
    _regular.append(Snippet("\\begin{remark}\n\t", "\\begin{remark}[]\n\t", 15, true));
    _regular.append(Snippet("\\begin{lemma}\n\t", "\\begin{lemma}[]\n\t", 14, true));
    _regular.append(Snippet("\\begin{example}\n\t", "\\begin{example}[]\n\t", 16, true));

    _regular.append(Snippet("doc", "\\documentclass{article}\n\\usepackage{lexpack}\n\n\\begin{document}\n\n\n\n\\end{document}\n", 64, true));
    _regular.append(Snippet("item", "\\begin{itemize}\n\t\\item \n\\end{itemize}\n", 23, true));
    _regular.append(Snippet("enum", "\\begin{enumerate}\n\t\\item \n\\end{enumerate}\n", 25, true));

    _math.append(Snippet("<<", "⟨"));
    _math.append(Snippet(">>", "⟩"));

    _math.append(Snippet("NN", "ℕ"));
    _math.append(Snippet("ZZ", "ℤ"));
    _math.append(Snippet("QQ", "ℚ"));
    _math.append(Snippet("RR", "ℝ"));
    _math.append(Snippet("CC", "ℂ"));
    _math.append(Snippet("PP", "ℙ"));

    _math.append(Snippet("..-", "⋯"));
    _math.append(Snippet("..|", "⋮"));
    _math.append(Snippet("..\\", "⋱"));

    _math.append(Snippet("for", "∀"));
    _math.append(Snippet("exi", "∃"));
    _math.append(Snippet("nn", "∩"));
    _math.append(Snippet("uu", "∪"));
    _math.append(Snippet("⊂=", "⊆"));
    _math.append(Snippet("cc", "⊂"));
    _math.append(Snippet("⊃=", "⊇"));
    _math.append(Snippet("pp", "⊃"));

    _math.append(Snippet("ll", "≪"));
    _math.append(Snippet("gg", "≫"));
    _math.append(Snippet("le", "≤"));
    _math.append(Snippet("ge", "≥"));

    _math.append(Snippet("⇐>", "⇔"));
    _math.append(Snippet("=>", "⇒"));
    _math.append(Snippet("<=", "⇐"));
    _math.append(Snippet("←>", "↔"));
    _math.append(Snippet("->", "→"));
    _math.append(Snippet("<-", "←"));

    _math.append(Snippet("xx", "×"));
    _math.append(Snippet("∘@", "@"));
    _math.append(Snippet("@", "∘"));
    _math.append(Snippet("+-", "±"));
    _math.append(Snippet("⋅*", "*"));
    _math.append(Snippet("**", "⋅⋅⋅"));
    _math.append(Snippet("*", "⋅"));
    _math.append(Snippet("o.", "⊙"));
    _math.append(Snippet("o+", "⊕"));
    _math.append(Snippet("ox", "⊗"));

    _math.append(Snippet("par", "∂"));
    _math.append(Snippet("oo", "∞"));

    _math.append(Snippet("sin", "sin")); // sin is not s\in
    _math.append(Snippet("!ni", "∌"));
    _math.append(Snippet("!in", "∉"));
    _math.append(Snippet("in", "∈"));
    _math.append(Snippet("ni", "∋"));

    _math.append(Snippet("==", "≡"));
    _math.append(Snippet("!=", "≠"));
    _math.append(Snippet("~~", "≈"));
    _math.append(Snippet("~=", "≅"));

    _math.append(Snippet("sum", "∑"));
    _math.append(Snippet("prod", "∏"));
    _math.append(Snippet("∈t", "∫"));
    _math.append(Snippet("∫i", "∫_{-∞}^{+∞}"));

    _math.append(Snippet("!O", "∅"));

    _math.append(Snippet("alp", "α"));
    _math.append(Snippet("bet", "β"));
    _math.append(Snippet("Gam", "Γ"));
    _math.append(Snippet("gam", "γ"));
    _math.append(Snippet("Del", "Δ"));
    _math.append(Snippet("del", "δ"));
    _math.append(Snippet("eps", "ɛ")); // varepsilon
    _math.append(Snippet("zet", "ζ"));
    _math.append(Snippet("eta", "η"));
    _math.append(Snippet("The", "Θ"));
    _math.append(Snippet("the", "ϑ")); // vartheta
    _math.append(Snippet("iot", "ι"));
    _math.append(Snippet("kap", "κ"));
    _math.append(Snippet("Lam", "Λ"));
    _math.append(Snippet("lam", "λ"));
    _math.append(Snippet("mu", "μ"));
    _math.append(Snippet("nu", "ν"));
    _math.append(Snippet("Xi", "Ξ"));
    _math.append(Snippet("xi", "ξ"));
    _math.append(Snippet("Pi", "Π"));
    _math.append(Snippet("pi", "π"));
    _math.append(Snippet("rho", "ρ"));
    _math.append(Snippet("Sig", "Σ"));
    _math.append(Snippet("sig", "σ"));
    _math.append(Snippet("tau", "τ"));
    _math.append(Snippet("ups", "υ"));
    _math.append(Snippet("Phi", "Φ"));
    _math.append(Snippet("phi", "φ")); // varphi
    _math.append(Snippet("chi", "χ"));
    _math.append(Snippet("Psi", "Ψ"));
    _math.append(Snippet("psi", "ψ"));
    _math.append(Snippet("Ome", "Ω"));
    _math.append(Snippet("ome", "ω"));
}

void SnippetManager::apply(Editor *editor, bool regular, bool math)
{
    QVector<Snippet> *snippets = nullptr;
    if (math && editor->markup(editor->caret() - 1) == Interval::Mathematics)
        snippets = &_math;
    else if (regular)
        snippets = &_regular;
    else
        return;

    for (const auto &snippet : *snippets)
        if (snippet.apply(editor))
            break;
}
