#pragma once

class Bird {
public:
    virtual double getDiameter() = 0;

private:
};

class NormalBird: public Bird {
public:
    NormalBird() {}
    double getDiameter() const {
        return 10;
    }

private:
};