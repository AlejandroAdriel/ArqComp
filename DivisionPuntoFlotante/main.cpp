#include <iostream>
using namespace std;

unsigned int floatBits(float f) {
    return *reinterpret_cast<unsigned int*>(&f);
}

void decomponerFloat(float f, unsigned int& signo, unsigned int& exponente, unsigned int& significando, bool& es_cero) {
    unsigned int bits = floatBits(f);

    signo = (bits >> 31) & 0x1;
    exponente = (bits >> 23) & 0xFF;
    significando = bits & 0x7FFFFF;
    es_cero = (bits & 0x7FFFFFFF) == 0;
}

float armarFloat(unsigned int signo, unsigned int exponente, unsigned int significando) {
    unsigned int bits = (signo << 31) | (exponente << 23) | (significando & 0x7FFFFF);
    return *reinterpret_cast<float*>(&bits);
}

void printFloatBits(float f) {
    unsigned int bits = floatBits(f);

    //signo
    cout <<((bits >> 31) & 0x1) << " ";
    //exponente
    for (int i = 30; i >= 23; i--) {
        cout << ((bits >> i) & 0x1);
    }
    cout << " ";
    //significando
    for (int i = 22; i >= 0; i--) {
        cout << ((bits >> i) & 0x1);
    }
    cout << endl;
}

float divisionFloat(float a, float b) {
    unsigned int a_signo, a_exponente, a_significando, b_signo, b_exponente, b_significando;
    bool a_cero, b_cero;

    decomponerFloat(a, a_signo, a_exponente, a_significando, a_cero);
    decomponerFloat(b, b_signo, b_exponente, b_significando, b_cero);

    if (b_cero) {
        return a_cero ? 0.0f/0.0f : (a_signo ^ b_signo) ? -1.0f/0.0f : 1.0f/0.0f;
    }
    if (a_cero) {
        return 0.0f;
    }

    unsigned int signo_resultado = a_signo ^ b_signo;

    int exponente_resultado = (int)a_exponente - (int)b_exponente + 127;

    if (exponente_resultado <= 0) {
        // Underflow - devolver cero
        return 0.0f;
    } else if (exponente_resultado >= 0xFF) {
        // overflow - devolver infinito
        return signo_resultado ? -1.0f/0.0f : 1.0f/0.0f;
    }

    unsigned int a_sign = a_significando | 0x800000;
    unsigned int b_sign = b_significando | 0x800000;

    unsigned long long dividendo = (unsigned long long)a_sign << 23;
    unsigned int cociente = (unsigned int)(dividendo / b_sign);

    if (cociente & 0x1000000) {
        cociente >>= 1;
        exponente_resultado++;
    } else if (!(cociente & 0x800000)) {
        cociente <<= 1;
        exponente_resultado--;
    }

    unsigned int significando_resultado = cociente & 0x7FFFFF;

    return armarFloat(signo_resultado,exponente_resultado,significando_resultado);
}

int main() {
    float a, b;

    cout << "Ingrese el dividendo: ";
    cin >> a;
    cout << "Ingrese el divisor: ";
    cin >> b;

    cout << "\nRepresentacion binaria: ";
    cout<<"\nDividendo ("<< a << "):" << endl;
    printFloatBits(a);

    cout << "\nDivisor (" << b << "):" << endl;
    printFloatBits(b);

    float MiResultado = divisionFloat(a, b);
    cout << "\nResultados:"<<endl;
    cout<<"Mi division: " << MiResultado;
    cout << "\nRepresentacion binaria: ";
    printFloatBits(MiResultado);

    float ResultadoNativo = a / b;
    cout << "\nDivision nativa: " << ResultadoNativo;
    cout << "\nRepresentacion binaria: ";
    printFloatBits(ResultadoNativo);

    cout << "\nDiferencia: " << MiResultado-ResultadoNativo << endl;

    return 0;
}
