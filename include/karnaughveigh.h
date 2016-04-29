#ifndef KARNAUGHVEIGH_H
#define KARNAUGHVEIGH_H

class Karnaughveigh {
public:
    Karnaughveigh(dimension);
    ~Karnaughveigh();
    void printAsTable();
    void insertElementAt();
    void deleteElementAt();

private:
    short dimension;

};

#endif // KARNAUGHVEIGH_H
