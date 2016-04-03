#ifndef SCORES_H
#define SCORES_H

#include <QGraphicsTextItem>


class Scores : public QGraphicsTextItem
{
public:
    Scores(QGraphicsItem* parent = 0);
    ~Scores();


    static Scores* instance()
    {
        if (!Score)
        {
            Score = new Scores;
        }
        return Score;
    }


    void getScores(int& scores);
    void incScores();

private:
    static Scores *Score;
    int scores;
};

#endif // SCORES_H
