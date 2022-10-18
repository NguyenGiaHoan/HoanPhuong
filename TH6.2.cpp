#include <stdio.h>
#include <stdlib.h>


struct THCCB 
{
	int so;
	int cao;
	struct THCCB *trai, *phai;
};


int max(int a, int b)
{
	if (a > b)
	   return a;
	else
		return b;   
}

int chieuCao(struct THCCB *N)
{
	if (N == NULL)
		return 0;
	return N->cao;
}

int layCanBang(struct THCCB *N)
{
	if (N == NULL)
		return 0;
	return chieuCao(N->trai) - chieuCao(N->phai);
}

struct THCCB *quayPhai(struct THCCB *y)
{
	struct THCCB *x = y->trai;
	struct THCCB *T2 = x->phai;

	x->phai = y;
	y->trai = T2;

	y->cao = max(chieuCao(y->trai), chieuCao(y->phai))+1;
	x->cao = max(chieuCao(x->trai), chieuCao(x->phai))+1;

	return x;
}

struct THCCB *quayTrai(struct THCCB *x)
{
	struct THCCB *y = x->phai;
	struct THCCB *T2 = y->trai;

	y->trai = x;
	x->phai = T2;

	x->cao = max(chieuCao(x->trai), chieuCao(x->phai))+1;
	y->cao = max(chieuCao(y->trai), chieuCao(y->phai))+1;
	return y;
}


struct THCCB* them(struct THCCB *goc, int x)
{
	/* 1.  Thuc hien them vao cay tim kiem nhi phan */
	if (goc == NULL)
	{
       goc = (struct THCCB*) malloc(sizeof(struct THCCB));
       goc->so = x;
       goc->cao = 1;
       goc->trai = goc->phai = NULL;
       return goc;
     }
	else
	   if (x < goc->so)
		goc->trai = them(goc->trai, x);
	   else if (x > goc->so)
		goc->phai = them(goc->phai, x);
	   else 
		return goc;

	/* 2. Cap nhat chieu cao */
	goc->cao = 1 + max(chieuCao(goc->trai), chieuCao(goc->phai));

	/* 3. Kiem tra can bang */

	int cb = layCanBang(goc);

	// Xet 4 truong hop 

	// Truong hop Trai-Trai
	if (cb > 1 && x < goc->trai->so)
		return quayPhai(goc);

	// Truong hop Phai-Phai
	if (cb < -1 && x > goc->phai->so)
		return quayTrai(goc);

	// Truong hop Trai-Phai
	if (cb > 1 && x > goc->trai->so)
	{
		goc->trai = quayTrai(goc->trai);
		return quayPhai(goc);
	}
	// Truong hop Phai-Trai
	if (cb < -1 && x < goc->phai->so)
	{
		goc->phai = quayPhai(goc->phai);
		return quayTrai(goc);
	}
	return goc;
}

//Ham lay so ngau nhien trong khoang tu min den max
int GetRandom(int min,int max)
{
    return min + (int)(rand()*(max-min+1.0)/(1.0+RAND_MAX));
}

//a. Kiem tra mot so nguyen x co thuoc TH S ko
int thuoc(int x, struct THCCB *S)
{
	if (S==NULL)
		return 0;
	else
	{
		if(S->so == x)
			return 1;
		else
		{
			if(S->so > x)
				return thuoc(x,S->trai);
			else
				return thuoc(x, S->phai);
		}
	}
}

//b. Liet ke cac phan tu trong tap hop
void inTHTruoc(struct THCCB *goc)
{
	if(goc != NULL)
	{
		printf("%d", goc->so);
		printf("\n");
		inTHTruoc(goc->trai);
		inTHTruoc(goc->phai);
	}
}
void inTHGiua(struct THCCB *goc)
{
	if(goc != NULL)
	{
		inTHGiua(goc->trai);
		printf("%d", goc->so);
		printf("\n");
		inTHGiua(goc->phai);
	}
}
void inTHSau(struct THCCB *goc)
{
	if(goc != NULL)
	{
		inTHSau(goc->trai);
		inTHSau(goc->phai);
		printf("%d", goc->so);
		printf("\n");
	}
}

//c. Kiem tra S1 la con S2 ko
int laTapCon(struct THCCB *S1, struct THCCB *S2)
{
	if (S1==NULL)
		return 1;
	else
		return laTapCon(S1->trai, S2) && 
				laTapCon(S1->phai, S2) && thuoc(S1->so, S2);
}

//d. Giao S1 va S2
struct THCCB* giao(struct THCCB *S1, struct THCCB *S2, struct THCCB *S3)
{
	if (S1!=NULL)
	{
		if (thuoc(S1->so, S2))
			S3 = them(S3, S1->so);
		S3 = giao(S1->trai, S2, S3);
		S3 = giao(S1->phai, S2, S3);
	}
	return S3;
}

//e. Hop S1 va S2
struct THCCB* themTH(struct THCCB *S1, struct THCCB *S2)
{
	if(S1!=NULL)
	{
		S2 = them(S2, S1->so);
		S2 = themTH(S1->trai, S2);
		S2 = themTH(S1->phai, S2);
	}
	return S2;
}
struct THCCB* hop(struct THCCB *S1, struct THCCB *S2)
{
	struct THCCB *S3 = NULL;
	S3 = themTH(S1, S3);
	S3 = themTH(S2, S3);
	return S3;
}

//f. Hieu S1 va S2
struct THCCB* hieu(struct THCCB *S1, struct THCCB *S2, THCCB *S3)
{
	if (S1!=NULL)
	{
		if (thuoc(S1->so, S2) == 0)
			S3 = them(S3, S1->so);
		S3 = hieu(S1->trai, S2, S3);
		S3 = hieu(S1->phai, S2, S3);
	}
	return S3;
}





int main()
{
	
		//Test voi du lieu nhieu
	struct THCCB *S1 = NULL;
	struct THCCB *S2 = NULL;
	struct THCCB *S3 = NULL;
	for(int i = 0; i < 1000000; i++)
	{
        int k = GetRandom(1, 1000000000);
        S1 = them(S1, k);
    }
    //inTHTruoc(gocS);
	for(int i = 0; i < 1000000; i++)
	{
        int k = GetRandom(1, 1000000);
        S2 = them(S2, k);
    }	
	return 0;
}
