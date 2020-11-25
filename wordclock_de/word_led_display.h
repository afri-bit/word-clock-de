/* 
 * Words and LEDs Descriptions
 *  o[122]                                                                               o[123]
 *
 *      	E[000] S[001] K[002] I[003] S[004] T[005] A[006] F[007] Ü[008] N[009] F[010]
 *      	Z[021] E[020] H[019] N[018] Z[017] W[016] A[015] N[014] Z[013] I[012] G[011]
 *      	D[022] R[023] E[024] I[025] V[026] I[027] E[028] R[029] T[030] E[031] L[032]
 *      	V[043] O[042] R[041] F[040] U[039] N[038] K[037] N[036] A[035] C[034] H[033]
 *      	H[044] A[045] L[046] B[047] A[048] E[049] L[050] F[051] Ü[052] N[053] F[054]
 *      	E[065] I[064] N[063] S[062] X[061] A[060] M[059] Z[058] W[057] E[056] I[055]
 *      	D[066] R[067] E[068] I[069] P[070] M[071] J[072] V[073] I[074] E[075] R[076]
 *      	S[087] E[086] C[085] H[084] S[083] N[082] L[081] A[080] C[079] H[078] T[077]
 *      	S[088] I[089] E[090] B[091] E[092] N[093] Z[094] W[095] Ö[096] L[097] F[098]
 *      	Z[109] E[108] H[107] N[106] E[105] U[104] N[103] K[102] U[101] H[100] R[099]
 * 
 *      	    o[110] o[111] o[112] o[113] o[114] o[115] o[116] o[117] o[118] o[119]      
 *  o[121]                                                                               o[120]
 * 
 * 
 *  Possible Times:
 *      01:00 --> Ein Uhr
 *      01:05 --> Fünf nach Eins
 *      01:10 --> Zehn nach Eins
 *      01:15 --> Viertel nach Eins / Viertel Zwei
 *      01:20 --> Zwanzig nach Eins
 *      01:25 --> Fünf vor halb Zwei
 *      01:30 --> Halb Zwei
 *      01:35 --> Fünf nach halb Zwei
 *      01:40 --> Zwanzig vor Zwei
 *      01:45 --> Viertel vor Zwei / Drei Viertel Zwei
 *      01:50 --> Zehn vor Zwei
 *      01:55 --> Fünf vor Zwei
 */

// ---------- Other LEDs functions




// Create a function to turn on LEDs from to 
void turnOnLEDFromTo(uint8_t from, uint8_t to, uint8_t red, uint8_t green, uint8_t blue)
{
    for(uint8_t i = from; i<=to; i++)
    {
        leds[i].setRGB(red, green, blue);
    }
}
// ---------- LEDs for Minutes

// ---------- LEDs for Seconds
void setSeconds(int second)
{

}

