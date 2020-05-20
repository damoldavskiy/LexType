#ifndef MATHWRITER_H
#define MATHWRITER_H

#include <QString>

/**
 * @brief LexType preprocessor (advanced math mode, fractions, etc.)
 */
class MathWriter
{
public:
    /**
     * @brief Passes text through preprocessor
     * @param Source
     * @return Result
     */
    static QString pass(const QString &source);

    /**
     * @brief Applies fractions to text
     * @param Source
     * @return Result
     */
    static QString applyFractions(QString source);

    /**
     * @brief Applies LaTeX parameters to text
     * @param Source
     * @return Result
     */
    static QString applyParameters(QString source);

    /**
     * @brief Applies advanced math mode to text
     * @param Source
     * @return Result
     */
    static QString apply(QString source);
};

#endif // MATHWRITER_H
