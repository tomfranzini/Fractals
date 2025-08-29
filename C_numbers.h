#ifndef C_NUMBERS_H_
#define C_NUMBERS_H_

#include<iostream>

class C_number{
    double re, im;

    public:
        C_number() : re(0), im(0) {};
        //~C_number();
        C_number(double x,double y) :re(x), im(y) {};
        C_number(const C_number& z){
            re = z.re;
            im = z.im;
        };

        void set_re(double x){
            re = x;
        };

        void set_im(double x){
            im = x;
        };

        operator double() const {
            if(im!=0)
                throw std::runtime_error("Cannot cast a complex number to double becasue Im is non-zero.\n");
            return re;
        };

        C_number operator+() const {
            return *this;
        };

        C_number operator-() const {
            return C_number(-re,-im);
        };

        C_number operator+(const C_number& z) const {
            return C_number(re+z.re,im+z.im);
        };

        C_number operator+(const double& a) const {
            return C_number(re+a,im);
        };

        C_number operator-(const double& a) const {
            return C_number(re-a,im);
        };

        C_number operator*(const double& a) const {
            return C_number(re*a,im*a);
        };

        C_number operator/(const double& a) const {
            if (a==0)
                throw std::runtime_error("You tried to devide by zero.\n");
            return C_number(re/a,im/a);
        };

        template<typename T> friend C_number operator+(T a, const C_number& z);
        template<typename T> friend C_number operator-(T a, const C_number& z);
        template<typename T> friend C_number operator*(T a, const C_number& z);
        template<typename T> friend C_number operator/(T a, const C_number& z);

        C_number operator-(const C_number& z){
           return C_number(re-z.re,im-z.im);
        };

        C_number operator*(const C_number& z) const {
            return C_number(re*z.re - im*z.im,re*z.im + im*z.re);
        };

        C_number operator/(const C_number& z) const {
            if(z.square_norm()==0)
                throw std::runtime_error("You tried to divide by zero.\n");

            //Here multiply and divide by z.complex_conj
            double den = z.square_norm();
            C_number nom = (*this) * z.complex_conj();
            return C_number(nom.re / den , nom.im / den);
        };

        C_number& operator = (const C_number& other){
            if (this == &other)   // self-assignment check
                return *this;

            re = other.re;
            im = other.im;

            return *this;
        }


        friend std::ostream& operator<< (std::ostream &, const C_number&);

        C_number complex_conj() const {
            return C_number(re,-im);
        }

        double square_norm() const {
            return re*re + im*im;
        };

};

//Template definitions should be in the header file
//I mark this definition as inline, to avoid redefinition

template<typename T> inline C_number operator+(double a, const C_number& z) {
    return C_number(z.re+a,z.im);
};

template<typename T> inline C_number operator-(T a, const C_number& z) {
    return C_number(z.re-a,z.im);
};

template<typename T> inline C_number operator*(T a, const C_number& z) {
    return C_number(z.re*a,z.im*a);
};

template<typename T> inline C_number operator/(T a, const C_number& z) {
    if(z.square_norm() == 0)
        throw std::runtime_error("You tried to devide by zero.\n");

    double den = z.square_norm();
    C_number nom = a * z.complex_conj();
    return C_number(nom.re / den , nom.im / den);
};


#endif // C_NUMBERS_H_
