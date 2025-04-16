#include <iostream>
#include <boost/dynamic_bitset.hpp>
using namespace std;

boost::dynamic_bitset<> decimalToBinary(int num, size_t bits) {
    return num >= 0 ? boost::dynamic_bitset<>(bits, num): boost::dynamic_bitset<>(bits, (1 << bits) + num);
}

int binaryToDecimal(const boost::dynamic_bitset<>& bits) {
    return bits[bits.size()-1] ? -((~bits).to_ulong() + 1) : bits.to_ulong();
}

boost::dynamic_bitset<> leerBinario(size_t bits) {
    string entrada;
    cout << "Ingrese " << bits << " bits: ";

    while (true) {
        cin >> entrada;
        if (entrada.length() == bits && entrada.find_first_not_of("01") == string::npos) {
            boost::dynamic_bitset<> resultado(bits);
            for (size_t i = 0; i < bits; ++i) {
                if (entrada[i] == '1') {
                    resultado.set(bits - 1 - i);
                }
            }
            return resultado;
        }
        cout << "Entrada inválida. Ingrese exactamente " << bits << " bits: ";
    }
}

boost::dynamic_bitset<> multiplicar(boost::dynamic_bitset<> M, boost::dynamic_bitset<> Q) {
    size_t n = M.size();
    boost::dynamic_bitset<> A(n, 0);
    int Q_1 = 0;

    M.resize(n+1);
    if (M[n-1]) M.set(n);

    for (size_t i = 0; i < n; ++i) {
        bool Q0 = Q[0];
        if (Q0 != Q_1) {
            A = boost::dynamic_bitset<>(n, A.to_ulong() + (Q0 ? -M.to_ulong() : M.to_ulong()));
        }

        Q_1 = Q0;
        Q >>= 1;
        Q[n-1] = A[0];
        A >>= 1;
        A[n-1] = A[n-2];
    }

    boost::dynamic_bitset<> resultado(2*n);
    for (size_t i = 0; i < n; ++i) {
        resultado[i] = Q[i];
        resultado[i+n] = A[i];
    }
    return resultado;
}

void mostrarBinario(const boost::dynamic_bitset<>& bits) {
    for (int i = bits.size()-1; i >= 0; --i) {
        cout << bits[i];
    }
}

int main() {
    const int MIN_BITS = 1;

    int bits_input;
    size_t bits;

    while (true) {
        cout << "Ingrese el tamaño de bits a utilizar: ";

        if (!(cin >> bits_input)) {
            cout << "Error: Debe ingresar un numero válido.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        if (bits_input < MIN_BITS) {
            cout << "Error: El tamaño debe ser " << MIN_BITS << " bit(s) o mas.\n";
            continue;
        }

        bits = static_cast<size_t>(bits_input);
        break;
    }
    int opcion;
    cout << "\n1. Ingresar valores en decimal\n2. Ingresar valores en binario\nElija opcion: ";
    cin >> opcion;

    boost::dynamic_bitset<> M, Q;

    if (opcion == 1) {
        int num;
        cout << "\nPrimer numero: ";
        cin >> num;
        M = decimalToBinary(num, bits);

        cout << "Segundo numero: ";
        cin >> num;
        Q = decimalToBinary(num, bits);
    }
    else if (opcion == 2) {
        cout << "\nPrimer numero binario de " << bits << " bits):\n";
        M = leerBinario(bits);

        cout << "\nSegundo numero binario de " << bits << " bits):\n";
        Q = leerBinario(bits);
    }
    else {
        cout << "Opcion invalida. Debe ser 1 o 2.\n";
        return 1;
    }

    cout << "\nM: "; mostrarBinario(M);
    cout << " (" << binaryToDecimal(M) << ")\n";

    cout << "Q: "; mostrarBinario(Q);
    cout << " (" << binaryToDecimal(Q) << ")\n";

    auto resultado = multiplicar(M, Q);

    cout << "\nResultado (" << 2*bits << " bits): ";
    mostrarBinario(resultado);
    cout << "\nDecimal: " << binaryToDecimal(resultado) << endl;

    return 0;
}
