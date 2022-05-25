#include <iostream>
#include <fstream>
using namespace std;

ifstream f("abce.in");
ofstream g("abce.out");

//functie pentru maxim

int maxim(int x, int y)
{
    if(x>y)
    return x;
    else
    return y;
}

//clasa pentru arbore

class NOD
{
    public:
    int valoare;   //key
    int inaltime;  //heigth
    NOD *stanga;   //left
    NOD *dreapta;  //right
};

//functie pentru inaltime

int INALTIME(NOD *N)
{
    if (N==NULL)
        return 0;
    return N->inaltime;
}


//aici creez un nod


NOD *nodnascut(int val)
{
    NOD* nod = new NOD();
    nod -> valoare = val;
    nod -> stanga = NULL;
    nod -> dreapta = NULL;
    nod -> inaltime = 1;

    return (nod);
}

//rotatie la dreapta

// N1,N2,N3

NOD *RR(NOD *y)
{
    NOD *x = y -> stanga;
    NOD *N2 = y -> dreapta;

    x -> dreapta = y;
    y -> stanga = N2;

    y -> inaltime = maxim(INALTIME(y -> stanga), INALTIME(y -> dreapta)) + 1;

    x -> inaltime = maxim(INALTIME(x -> stanga), INALTIME(x -> dreapta)) + 1;

    return x;
}

NOD *LR(NOD *x)
{
    NOD *y = x -> dreapta;
    NOD *N2 = y -> stanga;

    y -> stanga = x;
    x -> dreapta = N2;

    x -> inaltime = maxim(INALTIME(x -> stanga), INALTIME(x -> dreapta)) + 1;

    y -> inaltime = maxim(INALTIME(y -> stanga), INALTIME(y -> dreapta)) + 1;

    return y;
}

int FACTOR(NOD *N)
{
    if ( N == NULL )
        return 0;
    return INALTIME(N -> stanga) - INALTIME(N -> dreapta);
}

NOD* INSERARE(NOD* nod, int val)
{
    if ( nod == NULL )
        return(nodnascut(val));

    if (val < nod -> valoare)
        nod -> stanga = INSERARE( nod -> stanga, val );
    else
        if ( val > nod -> valoare )
            nod -> dreapta = INSERARE( nod -> dreapta, val);
        else
            return nod;

    nod -> inaltime = 1 + maxim( INALTIME( nod -> stanga) , INALTIME( nod -> dreapta ));

    int echilibru = FACTOR(nod);

    //LL
    if( echilibru > 1 && val < nod -> stanga -> valoare )
        return RR(nod);

    //RR

    if( echilibru < -1 && val > nod -> dreapta -> valoare )
        return LR(nod);

    //LR

    if( echilibru > 1 && val > nod -> stanga -> valoare )
    {
        nod -> stanga = LR ( nod -> stanga );
        return RR(nod);
    }

    //RL

    if( echilibru < -1 && val < nod -> dreapta -> valoare )
    {
        nod -> dreapta = RR ( nod -> dreapta );
        return LR(nod);
    }

    return nod;
}

NOD * minv(NOD* nod)
{
    NOD* curent = nod;

    while (curent -> stanga != NULL)
        curent = curent -> stanga;

    return curent;
}

NOD* STERGE(NOD* radacina, int val)
{

    if (radacina == NULL)
        return radacina;

    if ( val < radacina->valoare )
        radacina->stanga = STERGE(radacina->stanga, val);

    else if( val > radacina->valoare )
        radacina->dreapta = STERGE(radacina->dreapta, val);

    else
    {
        if( (radacina->stanga == NULL) ||
            (radacina->dreapta == NULL) )
        {
            NOD *temp = radacina->stanga ?
                         radacina->stanga :
                         radacina->dreapta;

            if (temp == NULL)
            {
                temp = radacina;
                radacina = NULL;
            }
            else
            *radacina = *temp;
            free(temp);
        }
        else
        {
            NOD* temp = minv(radacina->dreapta);

            radacina->valoare = temp->valoare;

            radacina->dreapta = STERGE(radacina->dreapta,temp->valoare);
        }
    }

    if (radacina == NULL)
    return radacina;

    radacina -> inaltime = 1 + maxim( INALTIME(radacina -> stanga) , INALTIME(radacina -> dreapta));

    int echilibru = FACTOR(radacina);

    //LL
    if (echilibru > 1 && FACTOR(radacina->stanga) >= 0)
        return RR(radacina);

    //LR
    if (echilibru > 1 &&
        FACTOR(radacina->stanga) < 0)
    {
        radacina->stanga = LR(radacina->stanga);
        return RR(radacina);
    }

    //RR
    if (echilibru < -1 &&
        FACTOR(radacina->dreapta) <= 0)
        return LR(radacina);

    //RL
    if (echilibru < -1 &&
        FACTOR(radacina->dreapta) > 0)
    {
        radacina->dreapta = RR(radacina->dreapta);
        return LR(radacina);
    }

    return radacina;
}

void CAUTA(NOD* radacina, int val)
{
    if(radacina==NULL)
        {
            g<<"0";
            return ;
        }
    NOD* nod;
    nod=radacina;

    if(nod -> valoare == val)
        g<<"1";

    if(nod -> valoare > val) //mergem in stanga
        CAUTA(nod -> stanga,val);
    else
        if(nod -> valoare < val) //mergem in dreapta
        CAUTA(nod -> dreapta,val);
}

NOD* CAUTA_SPECIAL(NOD* radacina, int val)
{
    if(radacina==NULL)
            return 0;
    NOD* nod;
    nod=radacina;

    if(nod -> valoare == val)
        return radacina;

    if(nod -> valoare > val) //mergem in stanga
        CAUTA_SPECIAL(nod -> stanga,val);
    else
        if(nod -> valoare < val) //mergem in dreapta
        CAUTA_SPECIAL(nod -> dreapta,val);
}

int mic(NOD* nod,int val) // cel mai mare nr mai mic sau egal ->
{

    if( nod -> valoare > val && nod -> stanga == NULL && nod -> dreapta == NULL )
        return -1;

    if( nod -> valoare <= val )
        if( nod -> dreapta == NULL || nod -> dreapta -> valoare > val)
            return nod -> valoare;

    if( nod -> valoare > val )
    {
        nod = nod -> stanga;
        return mic(nod, val);
    }

    if( nod -> valoare < val )
    {
        nod = nod -> dreapta;
        return mic(nod, val);
    }


}

int mare(NOD* radacina,int val) // cel mai mic nr mai mare sau egal ->
{
    NOD *nod = radacina, *bun = NULL;


    while (nod)
    {

        if (nod -> valoare > val)
        {
            bun = nod;
            nod = nod -> stanga;
        }


        else if (nod -> valoare == val)
        {
            bun = nod;
            break;
        }

        else
            nod = nod -> dreapta;
    }

    if (bun != NULL)
       return bun -> valoare;

    return -1;
}
void INORDER(NOD* nod, int y)
{
    if (nod == NULL || nod -> valoare > y )
        return;

    INORDER(nod -> stanga, y);

    cout << nod -> valoare << " ";

    INORDER(nod -> dreapta, y);
}

NOD* Cauta_X(int x)
{
    while(x<=9999999999999999)
    {
        if(CAUTA_SPECIAL(NULL,x)!=0);
        {
            return CAUTA_SPECIAL(NULL,x);
            cout<<CAUTA_SPECIAL(NULL,x)->valoare;
            break;
        }
        x++;
    }
}


int main()
{

    NOD* radacina = NULL;

    radacina = INSERARE(radacina,10);
    CAUTA(radacina,110);
    radacina = INSERARE(radacina,20);
    radacina = INSERARE(radacina,30);
    radacina = INSERARE(radacina,40);



    cout<<mare(radacina,2);
    cout<<endl;
    INORDER(radacina,32);

    cout<<endl;

    cout<<Cauta_X(30);




    return 0;
}
