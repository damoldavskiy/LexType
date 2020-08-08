\documentclass{article}
\usepackage{lexpack}

\begin{document}

\section*{Введение в математический анализ}

\begin{picture}
	\draw [->] (2.02,-6.3) -- (7.42448,-6.3);
	\draw [->] (2.02,-6.3) -- (2.02,-1.81639);
	\draw (7.42,-6.31) node [above right] {`x`};
	\draw (2.02,-1.81) node [above right] {`y`};
	\draw (2.02,-6.3) .. controls (2.32,-5.64) and (2.62,-5.04) .. (2.88,-4.78) .. controls (3.14,-4.52) and (3.74,-4.36) .. (4.08,-4.42) .. controls (4.42,-4.48) and (5.06,-4.72) .. (5.4,-4.72) .. controls (5.74,-4.72) and (6.34,-4.5) .. (6.62,-4.32) .. controls (6.9,-4.14) .. (7.54,-3.68);
\end{picture}

\begin{theorem}[Локальная теорема Муавра-Лапласа]
	Если в схеме Бернулли `n` стремится к бесконечности, величина `p ∈ (0, 1)` постоянна, а величина `x_m = (m - np)/(sqrt{npq})` ограничена равномерно по `m` и `n` (то есть `∃ a, b : -∞ < a ≤ x_m ≤ x_m ≤ b < +∞`), то
	``P_n(m) = 1/sqrt{2πnpq} exp(-x^2_m/2)(1 + α_n(m))``
\end{theorem}

\begin{definition}
	`f : V × V → ℂ` называется полуторалинейной формой над комплексным векторным пространством, если `∀ x, y, z ∈ V, ∀ α, β ∈ ℂ`:
	\begin{enumerate}
		\item `f(α x + β y, z) = α f(x, z) + β f(y, z)`
		\item `f(x, α y + β z) = bar α f(x, y) + bar β f(x, z)`
	\end{enumerate}
\end{definition}

\begin{corollary}
	Матрицу унитарного линейного оператора можно унитарным преобразованием привести к виду `Σ`, т.е. `∀ U ∈ U(n)` справедливо: `U = PΣP^*`, где `P` -- унитарная.
\end{corollary}

\begin{proof}
	`x = Px', y = Py' ⇒ comment{x^TF bar y}{Запись полуторалинейной формы} = (Px')^TF`
\end{proof}

\begin{remark}
	``∫_-∞^+∞ t ⋅ e^((-t^2)/(2σ^2)) dt = 0``
\end{remark}

\begin{lemma}[Евклида]
	Если простое число `p` делит без остатка произведение двух целых чисел `x ⋅ y`, то `p` делит `x` или `y`.
\end{lemma}

\begin{example}
	 Тензор типа `(1, 1)` можно интерпретировать как линейные операторы на `V`.
\end{example}

\begin{remark}
	``\(1, 1; 0, 0) + \(1, 1; 0, 0) = \(2, 2; 0, 0)``
\end{remark}

\begin{example}
	``|x| = {{x, x ≥ 0; -x, text{иначе}}}``
\end{example}

\end{document}


