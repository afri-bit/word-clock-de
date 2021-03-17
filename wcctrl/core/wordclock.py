from wcctrl.config.user import UserConfig
from wcctrl.component.led.ws2812b import LEDStrip


class WordClock:
    """
    Words and LEDs Descriptions
    o[122]                                                                o[123]

    E[000] S[001] K[002] I[003] S[004] T[005] A[006] F[007] Ü[008] N[009] F[010]
    Z[021] E[020] H[019] N[018] Z[017] W[016] A[015] N[014] Z[013] I[012] G[011]
    D[022] R[023] E[024] I[025] V[026] I[027] E[028] R[029] T[030] E[031] L[032]
    V[043] O[042] R[041] F[040] U[039] N[038] K[037] N[036] A[035] C[034] H[033]
    H[044] A[045] L[046] B[047] A[048] E[049] L[050] F[051] Ü[052] N[053] F[054]
    E[065] I[064] N[063] S[062] X[061] A[060] M[059] Z[058] W[057] E[056] I[055]
    D[066] R[067] E[068] I[069] P[070] M[071] J[072] V[073] I[074] E[075] R[076]
    S[087] E[086] C[085] H[084] S[083] N[082] L[081] A[080] C[079] H[078] T[077]
    S[088] I[089] E[090] B[091] E[092] N[093] Z[094] W[095] Ö[096] L[097] F[098]
    Z[109] E[108] H[107] N[106] E[105] U[104] N[103] K[102] U[101] H[100] R[099]

       o[110] o[111] o[112] o[113] o[114] o[115] o[116] o[117] o[118] o[119]

    o[121]                                                                o[120]
    ----------------------------------------------------------------------------
    Possible Times and the german informal Grammar:
        01:00 --> Ein Uhr
        01:05 --> Fünf nach Eins
        01:10 --> Zehn nach Eins
        01:15 --> Viertel nach Eins
        01:20 --> Zwanzig nach Eins
        01:25 --> Fünf vor halb Zwei
        01:30 --> Halb Zwei
        01:35 --> Fünf nach halb Zwei
        01:40 --> Zwanzig vor Zwei
        01:45 --> Viertel vor Zwei
        01:50 --> Zehn vor Zwei
        01:55 --> Fünf vor Zwei
    """

    def __init__(self, user_cfg: UserConfig, led_strip: LEDStrip):
        self.__led_strip = led_strip
        self.__led_strip.begin()

        self.__user_cfg = user_cfg

        self.__show_clock = True

        self.__led_am = [59, 60]
        self.__led_pm = [70, 71]
        self.__led_before = [41, 42, 43]
        self.__led_after = [33, 34, 35, 36]
        self.__led_word_it_is = [0, 1, 3, 4, 5]
        self.__led_o_clock = [99, 100, 101]

        self.__led_hour_one = [63, 64, 65]  # Ein
        self.__led_hour_ones = [62, 63, 64, 65]  # Eins
        self.__led_hour_two = [55, 56, 57, 58]
        self.__led_hour_three = [66, 67, 68, 69]
        self.__led_hour_four = [73, 74, 75, 76]
        self.__led_hour_five = [51, 52, 53, 54]
        self.__led_hour_six = [83, 84, 85, 86, 87]
        self.__led_hour_seven = [88, 89, 90, 91, 92, 93]
        self.__led_hour_eight = [77, 78, 79, 80]
        self.__led_hour_nine = [103, 104, 105, 106]
        self.__led_hour_ten = [106, 107, 108, 109]
        self.__led_hour_eleven = [49, 50, 51]
        self.__led_hour_twelve = [94, 95, 96, 97, 98]

        self.__led_minute_five = [7, 8, 9, 10]
        self.__led_minute_ten = [18, 19, 20, 21]
        self.__led_minute_quarter = [26, 27, 28, 29, 30, 31, 32]
        self.__led_minute_twenty = [11, 12, 13, 14, 15, 16, 17]
        self.__led_minute_half = [44, 45, 46, 47]
        self.__led_minute_three_quarter = [22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32]

        self.__led_minute_precision = {
            0: [120, 121, 122, 123],
            1: [122],
            2: [122, 123],
            3: [120, 122, 123],
            4: [120, 121, 122, 123]
        }

        self.__led_second = [110, 111, 112, 113, 114, 115, 116, 117, 118, 119]

    def __set_second(self, second: int):
        second_interval = second % 10

        # Turn OFF all LEDs for second indicator
        self.__led_strip.turn_off_leds(self.__led_second)

        self.__led_strip.turn_on_led(self.__led_second[second_interval], self.__user_cfg.get_color_second())

    def __set_minute(self, minute: int):
        """
        Possible Times:
            01:00 --> Ein Uhr
            01:05 --> Fünf nach Eins
            01:10 --> Zehn nach Eins
            01:15 --> Viertel nach Eins / Viertel Zwei
            01:20 --> Zwanzig nach Eins
            01:25 --> Fünf vor halb Zwei
            01:30 --> Halb Zwei
            01:35 --> Fünf nach halb Zwei
            01:40 --> Zwanzig vor Zwei
            01:45 --> Viertel vor Zwei / Drei Viertel Zwei
            01:50 --> Zehn vor Zwei
            01:55 --> Fünf vor Zwei
        :param minute: Current minute from time
        :return:
        """
        color_active = self.__user_cfg.get_color_active_letter()

        if minute >= 55:
            self.__led_strip.turn_on_leds(self.__led_minute_five, color_active)
            self.__led_strip.turn_on_leds(self.__led_before, color_active)
        elif minute >= 50:
            self.__led_strip.turn_on_leds(self.__led_minute_ten, color_active)
            self.__led_strip.turn_on_leds(self.__led_before, color_active)
        elif minute >= 45:
            self.__led_strip.turn_on_leds(self.__led_minute_quarter, color_active)
            self.__led_strip.turn_on_leds(self.__led_before, color_active)
        elif minute >= 40:
            self.__led_strip.turn_on_leds(self.__led_minute_twenty, color_active)
            self.__led_strip.turn_on_leds(self.__led_before, color_active)
        elif minute >= 35:
            self.__led_strip.turn_on_leds(self.__led_minute_five, color_active)
            self.__led_strip.turn_on_leds(self.__led_minute_half, color_active)
            self.__led_strip.turn_on_leds(self.__led_after, color_active)
        elif minute >= 30:
            self.__led_strip.turn_on_leds(self.__led_minute_half, color_active)
        elif minute >= 25:
            self.__led_strip.turn_on_leds(self.__led_minute_five, color_active)
            self.__led_strip.turn_on_leds(self.__led_minute_half, color_active)
            self.__led_strip.turn_on_leds(self.__led_before, color_active)
        elif minute >= 20:
            self.__led_strip.turn_on_leds(self.__led_minute_twenty, color_active)
            self.__led_strip.turn_on_leds(self.__led_after, color_active)
        elif minute >= 15:
            self.__led_strip.turn_on_leds(self.__led_minute_quarter, color_active)
            self.__led_strip.turn_on_leds(self.__led_after, color_active)
        elif minute >= 10:
            self.__led_strip.turn_on_leds(self.__led_minute_ten, color_active)
            self.__led_strip.turn_on_leds(self.__led_after, color_active)
        elif minute >= 5:
            self.__led_strip.turn_on_leds(self.__led_minute_five, color_active)
            self.__led_strip.turn_on_leds(self.__led_after, color_active)

    def __set_minute_precision(self, minute: int):
        minute_precision = minute % 5

        if minute_precision == 0:
            self.__led_strip.turn_off_leds(self.__led_minute_precision[minute_precision])
        else:
            self.__led_strip.turn_on_leds(self.__led_minute_precision[minute_precision],
                                          self.__user_cfg.get_color_minute())

    def __set_hour(self, hour: int):
        color_active = self.__user_cfg.get_color_active_letter()
        if hour == 0:
            self.__led_strip.turn_on_leds(self.__led_hour_twelve, color_active)
        elif hour == 1:
            self.__led_strip.turn_on_leds(self.__led_hour_one, color_active)
        elif hour == 2:
            self.__led_strip.turn_on_leds(self.__led_hour_two, color_active)
        elif hour == 3:
            self.__led_strip.turn_on_leds(self.__led_hour_three, color_active)
        elif hour == 4:
            self.__led_strip.turn_on_leds(self.__led_hour_four, color_active)
        elif hour == 5:
            self.__led_strip.turn_on_leds(self.__led_hour_five, color_active)
        elif hour == 6:
            self.__led_strip.turn_on_leds(self.__led_hour_six, color_active)
        elif hour == 7:
            self.__led_strip.turn_on_leds(self.__led_hour_seven, color_active)
        elif hour == 8:
            self.__led_strip.turn_on_leds(self.__led_hour_eight, color_active)
        elif hour == 9:
            self.__led_strip.turn_on_leds(self.__led_hour_nine, color_active)
        elif hour == 10:
            self.__led_strip.turn_on_leds(self.__led_hour_ten, color_active)
        elif hour == 11:
            self.__led_strip.turn_on_leds(self.__led_hour_eleven, color_active)
        elif hour == 12:
            self.__led_strip.turn_on_leds(self.__led_hour_twelve, color_active)

    def show_clock(self, val: bool):
        self.__show_clock = val

    def set_time(self, hour: int, minute: int, second: int):
        """

        :param hour: 24 hours time format
        :param minute:
        :param second:
        :return:
        """

        # Turn off all LEDs
        self.__led_strip.turn_off_all_leds()

        if self.__show_clock:
            # Turn on background color
            self.__led_strip.turn_on_leds(list(range(0, 110)), self.__user_cfg.get_color_background())

            # Set meridiem color
            if hour >= 12:
                # Set PM
                self.__led_strip.turn_on_leds(self.__led_pm, self.__user_cfg.get_color_ampm())
            else:
                # Set AM
                self.__led_strip.turn_on_leds(self.__led_am, self.__user_cfg.get_color_ampm())

            if minute >= 25:
                self.__set_hour((hour % 12) + 1)
            else:
                self.__set_hour(hour % 12)

            self.__set_minute(minute)
            self.__set_minute_precision(minute)
            self.__set_second(second)

            # Turn on the word 'Es ist'
            self.__led_strip.turn_on_leds(self.__led_word_it_is, self.__user_cfg.get_color_active_letter())

            if minute < 5:
                # Turn on word 'Uhr'
                self.__led_strip.turn_on_leds(self.__led_o_clock, self.__user_cfg.get_color_active_letter())

        self.__led_strip.show()
