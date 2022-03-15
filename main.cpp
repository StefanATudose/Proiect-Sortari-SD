#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <cstring>
#include <fstream>
#include <algorithm>

using namespace std;

ifstream fin("teste.in");
ofstream fout("teste.out");

long long numar_teste, val_max, nr_elem, i, mediane[3];
long long aux, pivot, pozpivot;
long long stlocal, drlocal, duble;


vector <long long> numere;
vector <long long> vectaux;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

void testsort(vector <long long> & numere, long long nr_elem)
{
    bool sortat = true;
    for (long long i = 0; i < nr_elem-1; i++)
        if (numere[i] > numere[i+1])
            sortat = false;
    if (sortat)
        fout << "Sortarea a functionat cu succes!\n";
    else
        fout << "Sirul nu este sortat corect!\n";
}

void mergesort(vector <long long> & numere, long long st, long long dr, vector <long long> & vectaux)
{
    /*
    if (dr > 250000)
    {
        cout << "\nNu pot sorta atat de mult cu mergesort (limita: 250.000)\n";
        return;
    }*/
    if (st == dr-1)
    {
        if (numere[st] > numere[dr])
        {
            long long aux = numere[st];
            numere[st] = numere[dr];
            numere[dr] = aux;
        }
        return;
    }
    else if (st == dr)
        return;
    long long mijloc = st + (dr-st)/2;
    mergesort (numere, st, mijloc, vectaux);
    mergesort (numere, mijloc+1, dr, vectaux);
    long long i = st, j = mijloc+1, aux = 0;
    while (i <= mijloc && j <= dr)
        if (numere[i] < numere[j])
            vectaux[aux++] = numere[i++];
        else
            vectaux[aux++] = numere[j++];
    while (j <= dr)
        vectaux[aux++] = numere[j++];
    while (i <= mijloc)
        vectaux[aux++] = numere[i++];
    for (aux = 0; aux < dr-st+1; aux++)
        numere[aux+st] = vectaux[aux];

}

void RadixSort10(vector <long long> & numere, long long nr_elem, long long val_max)
{
    vector <long long> frecventa[10];
    int zece = 1, j, i, k;
    while (zece <= val_max)
    {
        for (i = 0; i < nr_elem; i++)
            frecventa[numere[i] / zece % 10].push_back(numere[i]);
        i = 0;
        for (j = 0; j < 10; j++)
        {
            k = 0;
            while (k < frecventa[j].size())
                numere[i++] = frecventa[j][k++];
            frecventa[j].clear();
        }
        zece *= 10;
    }
}

void CountSort(vector <long long> & numere, long long nr_elem, long long val_max)
{
    long long i;
    vector <long long> frecventa;
    frecventa.assign(val_max+1, 0);
    for ( i = 0; i < nr_elem; i++)
        frecventa[numere[i]]++;
    int poz = 0;

    for (i = 0; i <= val_max; i++)
        while (frecventa[i])
        {
            numere[poz++] = i;
            frecventa[i]--;
        }
}

void quicksort(vector <long long> & numere, long long stanga, long long dreapta)
{
    if (stanga < dreapta)
    {

        mediane[0] = uniform_int_distribution <long long>(stanga, dreapta)(rng);
        mediane[1] = uniform_int_distribution <long long>(stanga, dreapta)(rng);
        mediane[2] = uniform_int_distribution <long long>(stanga, dreapta)(rng);
        pivot = max(min(numere[mediane[1]],numere[mediane[2]]), min(max(numere[mediane[1]],numere[mediane[2]]),numere[mediane[0]]));
        if (pivot == numere[mediane[0]])
                pozpivot = mediane[0];
        if (pivot == numere[mediane[1]])
                pozpivot = mediane[1];
        if (pivot == numere[mediane[2]])
                pozpivot = mediane[2];

        ///pozpivot = uniform_int_distribution <long long>(stanga, dreapta)(rng);
        ///pivot = numere[pozpivot];
        ///partitie
        stlocal = stanga;
        drlocal = dreapta-1;
        numere[pozpivot] = numere[dreapta];
        numere[dreapta] = pivot;
        while (true)
        {
            if (pivot <= numere[drlocal])
                drlocal--;
            if (numere[stlocal] < pivot)
                stlocal++;
            else
                if (stlocal < drlocal)
                {
                    aux = numere[stlocal];
                    numere[stlocal] = numere[drlocal];
                    numere[drlocal] = aux;
                }
                else
                    break;
        }
        aux = numere[dreapta];
        numere[dreapta] = numere[stlocal];
        numere[stlocal] = aux;
        drlocal = dreapta;
        aux = 0;
        while(drlocal > stlocal)
        {
            if (numere[drlocal] == pivot)
            {
                aux++;
                numere[drlocal] = numere[dreapta-aux+1];
                numere[dreapta-aux+1] = pivot;
            }
            drlocal--;
        }
        duble = aux;
        while (aux)
        {
            numere[dreapta-aux+1] = numere[stlocal+aux];
            numere[stlocal+aux] = pivot;
            aux--;
        }

        quicksort(numere, stanga, stlocal-1);
        quicksort(numere, stlocal+1+duble, dreapta);
    }
}


void ShellSort(vector <long long> & numere, long long nr_elem)
{
    int i, j, aux;
    for (int gap = nr_elem / 2; gap > 0; gap /= 2)
    {
        for (i = gap; i < nr_elem; i++)
        {
            aux = numere[i];
            for (j = i; j >= gap && numere[j-gap] > aux; j -= gap)
                numere[j] = numere[j-gap];
            numere[j] = aux;
        }
    }
}

int main()
{
    fin >> numar_teste;
    while(numar_teste)
    {
        fin >> nr_elem >> val_max;
        fout << "\nViteza de sortare pentru " << nr_elem << " elemente, cu valori cuprinse intre 0 si " << val_max << " inclusiv:\n\n";
        for (i = 0; i <= 5; i++)
        {
            numere.assign(nr_elem, 0);
            for (long long i = 0; i < nr_elem; i++)
                numere[i] = uniform_int_distribution <long long>(0, val_max)(rng);
            if (i == 0)
            {
                auto start = chrono::steady_clock::now();
                sort(numere.begin(), numere.end());
                auto finis = chrono::steady_clock::now();
                fout << "Sortare nativa C++: " << chrono::duration_cast<chrono::milliseconds>(finis-start).count() << " milisecunde\n";
                testsort(numere, nr_elem);
            }
            if (i == 1)
            {
                vectaux.reserve(nr_elem);
                auto start = chrono::steady_clock::now();
                mergesort(numere, 0, nr_elem-1, vectaux);
                auto finis = chrono::steady_clock::now();
                fout << "MergeSort: " << chrono::duration_cast<chrono::milliseconds>(finis-start).count() << " milisecunde\n";
                testsort(numere, nr_elem);
            }
            if(i == 2)
            {
                auto start = chrono::steady_clock::now();
                RadixSort10(numere, nr_elem, val_max);
                auto finis = chrono::steady_clock::now();
                fout << "Radix Sort in baza 10: " << chrono::duration_cast<chrono::milliseconds>(finis-start).count() << " milisecunde\n";
                testsort(numere, nr_elem);
            }
            if(i == 3)
            {
                auto start = chrono::steady_clock::now();
                ShellSort(numere, nr_elem);
                auto finis = chrono::steady_clock::now();
                fout << "Shell Sort(original n/2 gap): " << chrono::duration_cast<chrono::milliseconds>(finis-start).count() << " milisecunde\n";
                testsort(numere, nr_elem);
            }
            if(i==4)
            {
                auto start = chrono::steady_clock::now();
                quicksort(numere, 0, nr_elem-1);
                auto finis = chrono::steady_clock::now();
                fout << "Quick Sort: " << chrono::duration_cast<chrono::milliseconds>(finis-start).count() << " milisecunde\n";
                testsort(numere, nr_elem);
            }
            if(i == 5)
            {
                auto start = chrono::steady_clock::now();
                CountSort(numere, nr_elem, val_max);
                auto finis = chrono::steady_clock::now();
                fout << "Count Sort: " << chrono::duration_cast<chrono::milliseconds>(finis-start).count() << " milisecunde\n";
                testsort(numere, nr_elem);
            }

        }
        numar_teste--;
    }
    return 0;
}
