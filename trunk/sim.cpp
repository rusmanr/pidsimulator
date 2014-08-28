#include <algorithm>
#include <vector>
#include <limits>
#include <cmath>

#include "sim.h"

static const double PERIOD = 0.200;

static inline double scale(double inLow, double inHigh,
                           double in,
                           double outLow, double outHigh) {
    double m = (outHigh - outLow) / (inHigh - inLow);
    return m * (in - inLow) + outLow;
}

static inline double timeLag(double nowValue, double targetValue, double TC) {
    double tc = std::min(PERIOD * TC, 1.0);
    return nowValue + tc * (targetValue - nowValue);
}

static inline double limitToRange(double in, double lowLimit, double highLimit) {
    return std::max(
                std::min(in, highLimit),
                lowLimit);
}

class Plant {
public:

    Plant() {
        mVoltage = 0;
        mCurrent = 0;
        mResistance = 4.5;
        mTime = 0;
    }

    void sim(double appliedVolts) {
        const double TC = 0.66;

        mTime += PERIOD;
        if (mTime > 15) {
            mResistance += 0.02;
        }

        mVoltage = timeLag(mVoltage, appliedVolts, TC);
        mCurrent = mVoltage / mResistance;
    }

    double getCurrentFlow() const {
        return mCurrent;
    }

private:
    double mVoltage;
    double mCurrent;
    double mResistance;
    double mTime;
};

class VoltageSource {
public:

    VoltageSource() {
        mMin = 0;
        mMax = +100;
        mVoltage = 0;
        mTime = 0;
    }

    void sim(double signal) {
        const double TC = 4;

        mTime += PERIOD;
        mVoltage = timeLag(mVoltage, scale(0, 1, signal, mMin, mMax), TC);
        mVoltage += 0.5 * std::cos(2 * M_PI * .75 * mTime);
    }

    double getVoltage() const {
        return mVoltage;
    }

private:
    double mMin;
    double mMax;
    double mVoltage;
    double mTime;
};

class Controller {
public:

    Controller(double kp, double ki, double kd) {
        mLastError = std::numeric_limits<double>::quiet_NaN();
        mIntegral = 0;
        mControlSignal = 0;
        mKp = kp;
        mKi = ki;
        mKd = kd;
    }

    void sim(double sp, double pv) {

        double error = sp - pv;

        mControlSignal = 0;
        if (mKp != 0) {
            mControlSignal += mKp * error;
        }

        if (!std::isnan(mLastError)) {
            if (mKi != 0) {
                mIntegral += (error + mLastError) / 2.0 * PERIOD;
                mControlSignal += mKi * mIntegral;
            }

            if (mKd != 0) {
                mControlSignal += mKd * (error - mLastError) / PERIOD;
            }
        }

        mControlSignal = limitToRange(mControlSignal, 0.0, 1.0);
        mLastError = error;
    }

    double getSignal() const {
        return mControlSignal;
    }

private:
    double mLastError;
    double mIntegral;
    double mControlSignal;
    double mKp, mKi, mKd;
};

std::vector<SimPoint> controlResponse(int iterations, double setpoint, double kp, double ki, double kd)
{
    std::vector<std::pair<double, double> > result(iterations);

    Plant plant;
    VoltageSource voltageSource;
    Controller controller(kp, ki, kd);

    for (int iter = 0; iter <= iterations; ++iter) {
        double time = iter * PERIOD;

        plant.sim(voltageSource.getVoltage());
        controller.sim(setpoint, plant.getCurrentFlow());
        voltageSource.sim(controller.getSignal());

        SimPoint point;
        point.first = time;
        point.second = plant.getCurrentFlow();
        result.push_back(point);
    }
    return std:: move(result);
}

//void plantStepResponse(ostream& out, int iterations) {
//    Plant plant;
//    out << "Time,Plant Step Response\n";
//    for (int iter = 0; iter <= iterations; ++iter) {
//        double time = iter * PERIOD;
//        plant.sim(1.0);
//        out << time << "," << plant.getCurrentFlow() << endl;
//    }
//}

//void voltageSourceResponse(ostream& out, int iterations) {
//    VoltageSource vs;
//    out << "Time,Voltage Step Response\n";
//    for (int iter = 0; iter <= iterations; ++iter) {
//        double time = iter * PERIOD;
//        vs.sim(0.75);
//        out << time << "," << vs.getVoltage() << endl;
//    }
//}

