#include "snippetmanager.h"

SnippetManager::SnippetManager(bool reset)
{
    if (reset)
        this->reset();
}

void SnippetManager::reset()
{
    _snippets.clear();

    _snippets.append(Snippet(true, "sssec", "\\subsubsection{}", 15, true));
    _snippets.append(Snippet(true, "ssec", "\\subsection{}", 12, true));
    _snippets.append(Snippet(true, "sec", "\\section{}", 9, true));

    _snippets.append(Snippet(true, "section*{", "section{", true));
    _snippets.append(Snippet(true, "section{", "section*{", true));

    _snippets.append(Snippet(true, "thm", "\\begin{theorem}\n\t\n\\end{theorem}\n", 17, true));
    _snippets.append(Snippet(true, "def", "\\begin{definition}\n\t\n\\end{definition}\n", 20, true));
    _snippets.append(Snippet(true, "prf", "\\begin{proof}\n\t\n\\end{proof}\n", 15, true));
    _snippets.append(Snippet(true, "cor", "\\begin{corollary}\n\t\n\\end{corollary}\n", 19, true));
    _snippets.append(Snippet(true, "rmk", "\\begin{remark}\n\t\n\\end{remark}\n", 16, true));
    _snippets.append(Snippet(true, "lem", "\\begin{lemma}\n\t\n\\end{lemma}\n", 15, true));
    _snippets.append(Snippet(true, "exm", "\\begin{example}\n\t\n\\end{example}\n", 17, true));

    _snippets.append(Snippet(true, "\\begin{theorem}\n\t","\\begin{theorem}[]\n\t", 16, true));
    _snippets.append(Snippet(true, "\\begin{definition}\n\t", "\\begin{definition}[]\n\t", 19, true));
    _snippets.append(Snippet(true, "\\begin{proof}\n\t", "\\begin{proof}[]\n\t", 14, true));
    _snippets.append(Snippet(true, "\\begin{corollary}\n\t", "\\begin{corollary}[]\n\t", 18, true));
    _snippets.append(Snippet(true, "\\begin{remark}\n\t", "\\begin{remark}[]\n\t", 15, true));
    _snippets.append(Snippet(true, "\\begin{lemma}\n\t", "\\begin{lemma}[]\n\t", 14, true));
    _snippets.append(Snippet(true, "\\begin{example}\n\t", "\\begin{example}[]\n\t", 16, true));

    _snippets.append(Snippet(true, "doc", "\\documentclass{article}\n\\usepackage{lexpack}\n\n\\begin{document}\n\n\n\n\\end{document}\n", 64, true));
    _snippets.append(Snippet(true, "item", "\\begin{itemize}\n\t\\item \n\\end{itemize}\n", 23, true));
    _snippets.append(Snippet(true, "enum", "\\begin{enumerate}\n\t\\item \n\\end{enumerate}\n", 25, true));

    _snippets.append(Snippet(false, "t", "text{}", 5, true));
    _snippets.append(Snippet(false, "c", "comment{}", 8, true));

    _snippets.append(Snippet(false, "<<", "⟨"));
    _snippets.append(Snippet(false, ">>", "⟩"));

    _snippets.append(Snippet(false, "NN", "ℕ"));
    _snippets.append(Snippet(false, "ZZ", "ℤ"));
    _snippets.append(Snippet(false, "QQ", "ℚ"));
    _snippets.append(Snippet(false, "RR", "ℝ"));
    _snippets.append(Snippet(false, "CC", "ℂ"));
    _snippets.append(Snippet(false, "PP", "ℙ"));

    _snippets.append(Snippet(false, "..-", "⋯"));
    _snippets.append(Snippet(false, "..|", "⋮"));
    _snippets.append(Snippet(false, "..\\", "⋱"));

    _snippets.append(Snippet(false, "for", "∀"));
    _snippets.append(Snippet(false, "exi", "∃"));
    _snippets.append(Snippet(false, "nn", "∩"));
    _snippets.append(Snippet(false, "uu", "∪"));
    _snippets.append(Snippet(false, "⊂=", "⊆"));
    _snippets.append(Snippet(false, "cc", "⊂"));
    _snippets.append(Snippet(false, "⊃=", "⊇"));
    _snippets.append(Snippet(false, "pp", "⊃"));

    _snippets.append(Snippet(false, "ll", "≪"));
    _snippets.append(Snippet(false, "gg", "≫"));
    _snippets.append(Snippet(false, "le", "≤"));
    _snippets.append(Snippet(false, "ge", "≥"));

    _snippets.append(Snippet(false, "⇐>", "⇔"));
    _snippets.append(Snippet(false, "=>", "⇒"));
    _snippets.append(Snippet(false, "<=", "⇐"));
    _snippets.append(Snippet(false, "←>", "↔"));
    _snippets.append(Snippet(false, "->", "→"));
    _snippets.append(Snippet(false, "<-", "←"));

    _snippets.append(Snippet(false, "xx", "×"));
    _snippets.append(Snippet(false, "∘@", "@"));
    _snippets.append(Snippet(false, "@", "∘"));
    _snippets.append(Snippet(false, "+-", "±"));
    _snippets.append(Snippet(false, "⋅*", "*"));
    _snippets.append(Snippet(false, "**", "⋅⋅⋅"));
    _snippets.append(Snippet(false, "*", "⋅"));
    _snippets.append(Snippet(false, "o.", "⊙"));
    _snippets.append(Snippet(false, "o+", "⊕"));
    _snippets.append(Snippet(false, "ox", "⊗"));

    _snippets.append(Snippet(false, "par", "∂"));
    _snippets.append(Snippet(false, "oo", "∞"));

    _snippets.append(Snippet(false, "sin", "sin")); // sin is not s\in
    _snippets.append(Snippet(false, "!ni", "∌"));
    _snippets.append(Snippet(false, "!in", "∉"));
    _snippets.append(Snippet(false, "in", "∈"));
    _snippets.append(Snippet(false, "ni", "∋"));

    _snippets.append(Snippet(false, "==", "≡"));
    _snippets.append(Snippet(false, "!=", "≠"));
    _snippets.append(Snippet(false, "~~", "≈"));
    _snippets.append(Snippet(false, "~=", "≅"));

    _snippets.append(Snippet(false, "sum", "∑"));
    _snippets.append(Snippet(false, "prod", "∏"));
    _snippets.append(Snippet(false, "∈t", "∫"));
    _snippets.append(Snippet(false, "∫i", "∫_-∞^+∞"));

    _snippets.append(Snippet(false, "!O", "∅"));

    _snippets.append(Snippet(false, "alp", "α"));
    _snippets.append(Snippet(false, "bet", "β"));
    _snippets.append(Snippet(false, "Gam", "Γ"));
    _snippets.append(Snippet(false, "gam", "γ"));
    _snippets.append(Snippet(false, "Del", "Δ"));
    _snippets.append(Snippet(false, "del", "δ"));
    _snippets.append(Snippet(false, "eps", "ɛ")); // varepsilon
    _snippets.append(Snippet(false, "zet", "ζ"));
    _snippets.append(Snippet(false, "eta", "η"));
    _snippets.append(Snippet(false, "The", "Θ"));
    _snippets.append(Snippet(false, "the", "ϑ")); // vartheta
    _snippets.append(Snippet(false, "iot", "ι"));
    _snippets.append(Snippet(false, "kap", "κ"));
    _snippets.append(Snippet(false, "Lam", "Λ"));
    _snippets.append(Snippet(false, "lam", "λ"));
    _snippets.append(Snippet(false, "mu", "μ"));
    _snippets.append(Snippet(false, "nu", "ν"));
    _snippets.append(Snippet(false, "Xi", "Ξ"));
    _snippets.append(Snippet(false, "xi", "ξ"));
    _snippets.append(Snippet(false, "Pi", "Π"));
    _snippets.append(Snippet(false, "pi", "π"));
    _snippets.append(Snippet(false, "rho", "ρ"));
    _snippets.append(Snippet(false, "Sig", "Σ"));
    _snippets.append(Snippet(false, "sig", "σ"));
    _snippets.append(Snippet(false, "tau", "τ"));
    _snippets.append(Snippet(false, "ups", "υ"));
    _snippets.append(Snippet(false, "Phi", "Φ"));
    _snippets.append(Snippet(false, "phi", "φ")); // varphi
    _snippets.append(Snippet(false, "chi", "χ"));
    _snippets.append(Snippet(false, "Psi", "Ψ"));
    _snippets.append(Snippet(false, "psi", "ψ"));
    _snippets.append(Snippet(false, "Ome", "Ω"));
    _snippets.append(Snippet(false, "ome", "ω"));
}

QVector<Snippet> &SnippetManager::snippets()
{
    return _snippets;
}

const QVector<Snippet> &SnippetManager::snippets() const
{
    return _snippets;
}

QDataStream &operator <<(QDataStream &stream, const SnippetManager &manager)
{
    stream << manager._snippets.size();
    for (const Snippet &snippet : manager._snippets)
        stream << snippet;

    return stream;
}

QDataStream &operator >>(QDataStream &stream, SnippetManager &manager)
{
    int size;
    stream >> size;
    manager._snippets.resize(size);
    for (int i = 0; i < size; ++i)
        stream >> manager._snippets[i];

    return stream;
}
