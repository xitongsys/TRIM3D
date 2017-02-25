#include <bits/stdc++.h>
#include "stopping.h"

using namespace std;

int main(){
	Stopping sp("SCOEF.88");


/*
//H_C
	double Z1=1,M1=1.008,Z2=6,M2=12.011,ARho=0.113;
	for(double e0=1000; e0<1e7; e0+=1000){//1keV-10MeV
		cout<<e0<<" "<<sp.hstop(Z1, M1, Z2, M2, e0/1000/M1)*ARho<<endl;
	}


//H_Si
	double Z1=1,M1=1.008,Z2=14,M2=28.086,ARho=0.04977;
	for(double e0=1000; e0<1e7; e0+=1000){//1keV-10MeV
		cout<<e0<<" "<<sp.hstop(Z1, M1, Z2, M2, e0/1000/M1)*ARho<<endl;
	}
*/

/*
	//C_Si
	double Z1=6,M1=12.011,Z2=14,M2=28.086,ARho=0.04977;
	for(double e0=1000; e0<1e7; e0+=1000){//1keV-10MeV
		cout<<e0<<" "<<sp.histop(Z1, M1, Z2, M2, e0/1000/M1, sp.atomInf[Z2][6], sp.atomInf[Z1][7])*ARho<<endl;
	}
*/

/*
	//O_C
	double Z1=8,M1=15.995,Z2=6,M2=12.011,ARho=0.11296;
	for(double e0=1000; e0<1e7; e0+=1000){//1keV-10MeV
		cout<<e0<<" "<<sp.histop(Z1, M1, Z2, M2, e0/1000/M1, sp.atomInf[Z2][6], sp.atomInf[Z1][7])*ARho<<endl;
	}
*/


/* 
//O_Al
	double Z1=8,M1=15.995,Z2=13,M2=26.982,ARho=0.060305;
	for(double e0=1000; e0<1e7; e0+=1000){//1keV-10MeV
		cout<<e0<<" "<<sp.histop(Z1, M1, Z2, M2, e0/1000/M1, sp.atomInf[Z2][6], sp.atomInf[Z1][7])*ARho<<endl;
	}

*/

/*
//He_Fe
	double Z1=2,M1=4.003,Z2=26,M2=55.847,ARho=0.084817;
	for(double e0=1000; e0<1e7; e0+=1000){//1keV-10MeV
		cout<<e0<<" "<<sp.histop(Z1, M1, Z2, M2, e0/1000/M1, sp.atomInf[Z2][6], sp.atomInf[Z1][7])*ARho<<endl;
	}

*/

//He_Si
	double Z1=2,M1=4.003,Z2=14,M2=28.086,ARho=0.04977;
	for(double e0=1000; e0<1e7; e0+=1000){//1keV-10MeV
		cout<<e0<<" "<<sp.histop(Z1, M1, Z2, M2, e0/1000/M1, sp.atomInf[Z2][6], sp.atomInf[Z1][7])*ARho<<endl;
	}




	return 0;
}


