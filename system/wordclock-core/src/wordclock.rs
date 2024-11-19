use std::collections::HashMap;

// Words and LEDs Descriptions
// o[122]                                                                o[123]

// E[000] S[001] K[002] I[003] S[004] T[005] A[006] F[007] Ü[008] N[009] F[010]
// Z[021] E[020] H[019] N[018] Z[017] W[016] A[015] N[014] Z[013] I[012] G[011]
// D[022] R[023] E[024] I[025] V[026] I[027] E[028] R[029] T[030] E[031] L[032]
// V[043] O[042] R[041] F[040] U[039] N[038] K[037] N[036] A[035] C[034] H[033]
// H[044] A[045] L[046] B[047] A[048] E[049] L[050] F[051] Ü[052] N[053] F[054]
// E[065] I[064] N[063] S[062] X[061] A[060] M[059] Z[058] W[057] E[056] I[055]
// D[066] R[067] E[068] I[069] P[070] M[071] J[072] V[073] I[074] E[075] R[076]
// S[087] E[086] C[085] H[084] S[083] N[082] L[081] A[080] C[079] H[078] T[077]
// S[088] I[089] E[090] B[091] E[092] N[093] Z[094] W[095] Ö[096] L[097] F[098]
// Z[109] E[108] H[107] N[106] E[105] U[104] N[103] K[102] U[101] H[100] R[099]

// o[110] o[111] o[112] o[113] o[114] o[115] o[116] o[117] o[118] o[119]

// o[121]                                                                o[120]
// ----------------------------------------------------------------------------
// Possible Times and the german informal Grammar:
// 01:00 --> Ein Uhr
// 01:05 --> Fünf nach Eins
// 01:10 --> Zehn nach Eins
// 01:15 --> Viertel nach Eins
// 01:20 --> Zwanzig nach Eins
// 01:25 --> Fünf vor halb Zwei
// 01:30 --> Halb Zwei
// 01:35 --> Fünf nach halb Zwei
// 01:40 --> Zwanzig vor Zwei
// 01:45 --> Viertel vor Zwei
// 01:50 --> Zehn vor Zwei
// 01:55 --> Fünf vor Zwei
struct WordClock {
    led_mapper: HashMap<&'static str, Vec<u8>>,
}

impl WordClock {
    pub fn new() -> Self {
        let led_mapper: HashMap<&'static str, Vec<u8>> = HashMap::from([
            ("am", vec![59, 60]),
            ("pm", vec![70, 71]),
            ("before", vec![41, 42, 43]),
            ("after", vec![33, 34, 35, 36]),
            ("it_is", vec![0, 1, 3, 4, 5]),
            ("o_clock", vec![99, 100, 101]),
            ("hour_one", vec![63, 64, 65]),      // Ein
            ("hour_ones", vec![62, 63, 64, 65]), // Eins
            ("hour_two", vec![55, 56, 57, 58]),
            ("hour_three", vec![66, 67, 68, 69]),
            ("hour_four", vec![73, 74, 75, 76]),
            ("hour_five", vec![51, 52, 53, 54]),
            ("hour_six", vec![83, 84, 85, 86, 87]),
            ("hour_seven", vec![88, 89, 90, 91, 92, 93]),
            ("hour_eight", vec![77, 78, 79, 80]),
            ("hour_nine", vec![103, 104, 105, 106]),
            ("hour_ten", vec![106, 107, 108, 109]),
            ("hour_eleven", vec![49, 50, 51]),
            ("hour_twelve", vec![94, 95, 96, 97, 98]),
            ("minute_five", vec![7, 8, 9, 10]),
            ("minute_ten", vec![18, 19, 20, 21]),
            ("minute_quarter", vec![26, 27, 28, 29, 30, 31, 32]),
            ("minute_twenty", vec![11, 12, 13, 14, 15, 16, 17]),
            ("minute_half", vec![44, 45, 46, 47]),
            (
                "minute_three_quarter",
                vec![22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32],
            ),
            ("minute_pres_0", vec![120, 121, 122, 123]),
            ("minute_pres_1", vec![122]),
            ("minute_pres_2", vec![122, 123]),
            ("minute_pres_3", vec![120, 122, 123]),
            ("minute_pres_4", vec![120, 121, 122, 123]),
            (
                "second",
                vec![110, 111, 112, 113, 114, 115, 116, 117, 118, 119],
            ),
        ]);

        WordClock { led_mapper }
    }
}
