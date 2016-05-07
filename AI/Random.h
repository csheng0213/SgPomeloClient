#ifndef _RANDOM_H_
#define _RANDOM_H_

class Random
{
public:
    Random()
        : _seed(0xffff)
    {}

    unsigned int rand()
    {
        return (_seed = (0x7afbfcfd * _seed + 0x7258be1d) % 0x80000000);
    }

    float randf()
    {
        return (float)rand() / 0x80000000;
    }

    float randmf()
    {
        return (float)rand() / 0x40000000 - 1.0f;
    }

    void srand(unsigned int seed)
    {
        _seed = seed;
    }

private:
    unsigned int _seed;
};

#endif // _RANDOM_H_