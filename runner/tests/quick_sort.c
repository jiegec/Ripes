#include "lib.h"

int partition(int* a, int p, int q) {
    int pivot = a[p];
    int i = p, j = q;
    while (i < j) {
        while (i < j && a[j] > pivot)
            j--;
        a[i] = a[j];

        while (i < j && a[i] <= pivot)
            i++;
        a[j] = a[i];
    }

    a[i] = pivot;
    return i;
}

int result[1000];
void _quick_sort(int* a, int p, int q) {
    if (p >= q)
        return;
    int m = partition(a, p, q);
    _quick_sort(a, p, m - 1);
    _quick_sort(a, m + 1, q);
}
int* quick_sort(int* a, int N) {
    int i;
    for (i = 0; i < N; i++) {
        result[i] = a[i];
    }
    _quick_sort(result, 0, N - 1);
    return result;
}

#define N 1000

static int a[N] = {
    18361, 25784, 6605,  26590, 22618, 15285, 27647, 23843, 16362, 10047, 16750, 10588, 27793, 20888, 27352, 6613,
    6426,  9475,  1198,  17530, 23329, 31946, 17543, 42,    21700, 8569,  15002, 4705,  12988, 27549, 19075, 24352,
    19489, 17567, 12310, 5363,  30347, 17034, 31029, 8821,  27399, 14673, 5216,  15979, 26439, 29891, 25161, 14524,
    18559, 13036, 9790,  18401, 2223,  16069, 7917,  4284,  10022, 26104, 14827, 2867,  18285, 1073,  26236, 24110,
    10506, 24816, 24846, 6813,  21026, 27498, 1618,  9173,  3264,  20262, 28522, 9796,  22268, 23679, 21324, 14882,
    19599, 21158, 12181, 5101,  25414, 6808,  8534,  9282,  10802, 27835, 28598, 10331, 4015,  19630, 20789, 32664,
    22681, 11411, 29913, 25229, 6165,  3426,  17535, 31632, 19593, 30505, 15223, 2318,  14036, 20900, 7556,  25432,
    16015, 8758,  4713,  26187, 19900, 5130,  16473, 19631, 32670, 24508, 20314, 7631,  13785, 16126, 20449, 28518,
    9821,  15831, 3234,  30579, 19535, 22706, 4768,  1923,  18616, 13435, 32295, 28430, 543,   30101, 4202,  15992,
    28713, 20597, 14893, 27908, 20386, 9488,  27977, 27549, 9303,  24865, 25073, 19337, 11325, 5301,  12619, 24167,
    23461, 21817, 8405,  4121,  29052, 6111,  24844, 24437, 31551, 3907,  9170,  16647, 27220, 15102, 9530,  2249,
    24399, 18465, 5493,  17119, 20611, 20390, 19773, 28,    18161, 6403,  12863, 1593,  10843, 22910, 4724,  11134,
    4153,  5452,  18690, 16416, 1906,  24535, 16446, 23820, 19100, 28521, 27774, 20718, 14827, 12463, 23005, 25687,
    16010, 29026, 25342, 5468,  28703, 15710, 27848, 32283, 26420, 15317, 18639, 21943, 19840, 17255, 28696, 24357,
    20606, 9486,  937,   4160,  4981,  7019,  22000, 17665, 31013, 4260,  10617, 30818, 18182, 7565,  16832, 20498,
    7177,  14293, 19961, 24223, 18518, 25822, 22471, 8,     19380, 23623, 2202,  21965, 21102, 26895, 7429,  30068,
    21648, 2980,  335,   5098,  6690,  29231, 6012,  9279,  356,   30010, 31377, 8133,  28387, 18782, 2760,  9452,
    23837, 17550, 29237, 7372,  20642, 3499,  32400, 16192, 12653, 12849, 21572, 30565, 18887, 22554, 21152, 15598,
    22057, 30934, 23777, 24037, 5602,  24896, 22587, 15571, 7570,  8792,  27830, 28464, 2113,  5274,  19172, 26597,
    4048,  31537, 30282, 14226, 5963,  3575,  32162, 15801, 28447, 140,   14435, 2747,  15940, 4298,  28193, 23730,
    22763, 22864, 28774, 26263, 3902,  27368, 19678, 18963, 21464, 13172, 10679, 23995, 26474, 1846,  8759,  2212,
    32551, 599,   24111, 14573, 9568,  32754, 3615,  13647, 5624,  3270,  24971, 2412,  23450, 31302, 13737, 10042,
    4344,  23792, 21651, 9744,  1118,  22727, 9418,  27218, 28384, 741,   9552,  24236, 4217,  18331, 26948, 18266,
    7759,  6084,  29276, 17319, 17591, 7008,  3064,  29484, 8365,  29089, 14748, 20762, 20371, 31222, 18637, 10655,
    12153, 25911, 14817, 760,   6243,  26599, 5373,  15219, 11143, 30279, 23782, 10629, 1498,  2582,  27952, 4692,
    26974, 25458, 6180,  3701,  32659, 17635, 20402, 2849,  25274, 24304, 9406,  23357, 18562, 28701, 7710,  25780,
    1524,  2137,  11351, 20735, 13376, 29907, 7599,  29598, 23772, 18826, 20662, 14849, 20621, 5702,  8980,  11544,
    16812, 14580, 30914, 24374, 10060, 12695, 30240, 26933, 23265, 15744, 17352, 17498, 20135, 4304,  20,    18077,
    16624, 3301,  26416, 10765, 5235,  15650, 14828, 31201, 28773, 15363, 12074, 11213, 19463, 12216, 7626,  28333,
    17656, 19567, 18211, 2213,  16818, 17937, 18840, 24196, 29115, 19086, 3145,  22418, 3758,  331,   8881,  27051,
    13753, 14492, 21686, 5620,  29542, 2811,  14049, 11791, 31374, 8446,  21295, 171,   11406, 26004, 4959,  31477,
    9895,  27717, 26014, 10630, 30780, 2079,  28466, 26714, 6106,  17590, 24342, 29043, 1976,  25331, 29158, 1583,
    12928, 6071,  17581, 28865, 13210, 28009, 20784, 27309, 19499, 25346, 22921, 28565, 24371, 20989, 25661, 12137,
    17434, 16928, 2064,  7383,  16781, 22027, 3396,  4464,  6054,  12919, 19402, 3712,  6782,  32083, 9418,  32479,
    3305,  24726, 19060, 12909, 20922, 19190, 12266, 31608, 19404, 31581, 22331, 3954,  23414, 28587, 27314, 6131,
    5511,  29261, 19165, 17151, 12790, 20667, 20072, 24464, 8549,  8734,  9563,  13697, 23370, 7305,  4337,  5331,
    32101, 5627,  26032, 20949, 9635,  7494,  1480,  21359, 28744, 23256, 25455, 23379, 72,    8557,  20315, 6111,
    28786, 27823, 32726, 5890,  23936, 15676, 26132, 23521, 21952, 21332, 23018, 25613, 22490, 19980, 18174, 29259,
    21929, 32663, 27343, 30842, 14552, 5854,  5961,  4301,  21307, 5022,  3953,  2151,  22275, 10797, 4218,  27467,
    10812, 31936, 26645, 29079, 14373, 9988,  12351, 14128, 25859, 20555, 12644, 23079, 30907, 32457, 2368,  31622,
    25750, 31918, 11738, 1921,  32209, 31463, 8812,  4015,  29293, 11332, 19249, 7770,  27392, 28268, 9899,  30045,
    19217, 3801,  28002, 32616, 9388,  28039, 23772, 10169, 1520,  31886, 1950,  15888, 14119, 5672,  23563, 2543,
    21167, 3185,  10804, 27142, 26312, 347,   18389, 4863,  21184, 26380, 2864,  7285,  7884,  9476,  29547, 11557,
    7128,  22472, 16696, 24750, 31432, 14132, 27344, 10428, 457,   31256, 24076, 4174,  18715, 14206, 2315,  26588,
    20993, 25397, 31903, 30534, 25304, 12516, 1984,  5463,  9284,  19233, 12060, 32211, 22643, 13078, 9933,  3340,
    22387, 7052,  10172, 29558, 7875,  28161, 23737, 6773,  16088, 7629,  9709,  7218,  14952, 19326, 1547,  27570,
    7565,  19076, 23355, 17078, 2736,  28025, 27888, 8276,  17470, 6836,  16311, 20254, 4378,  27687, 6910,  15950,
    7444,  14909, 9320,  30507, 897,   14089, 3394,  11101, 4647,  32438, 14317, 26071, 11819, 12056, 12029, 2749,
    18620, 26555, 21313, 5899,  29927, 3681,  18781, 6492,  6078,  27068, 25942, 12691, 296,   3191,  227,   387,
    24916, 4274,  16784, 8813,  18832, 16046, 14526, 3237,  28320, 6649,  28356, 23302, 8740,  15353, 31081, 4438,
    597,   22616, 19714, 5051,  31653, 26512, 26618, 28501, 24151, 23735, 9786,  22530, 12114, 31329, 12084, 16310,
    6264,  2935,  26102, 18118, 12128, 29778, 30669, 26863, 30002, 28063, 15537, 11253, 797,   1977,  12794, 21066,
    7896,  12653, 4165,  25848, 4475,  22378, 6214,  22527, 17868, 10283, 31710, 791,   11574, 24006, 8950,  2544,
    8659,  19087, 4920,  6184,  13590, 25766, 2813,  15107, 2049,  17820, 24626, 3792,  16481, 15923, 12153, 21888,
    619,   27607, 11335, 9571,  11509, 15583, 32416, 26258, 6498,  10724, 31493, 23770, 17050, 16073, 13640, 14960,
    13997, 15455, 14500, 14277, 7179,  26554, 459,   30202, 11643, 1952,  17685, 3079,  21575, 19132, 14610, 31624,
    25251, 23470, 13038, 23314, 13782, 16378, 21553, 9029,  11935, 2603,  24128, 26285, 1658,  29441, 3681,  14540,
    31382, 24257, 4221,  13759, 15049, 1055,  23316, 15803, 32617, 25773, 17756, 27469, 18021, 15717, 27446, 21166,
    26014, 10474, 27290, 29485, 18048, 11796, 27532, 29054, 10298, 6384,  27273, 7218,  18523, 32454, 11145, 20550,
    31100, 4945,  23427, 11621, 30388, 19151, 11485, 2842,  2086,  18222, 27660, 20772, 17907, 877,   16612, 4300,
    28980, 12827, 19865, 13052, 6848,  28159, 17392, 27934, 3305,  21843, 27323, 28800, 16178, 7090,  27367, 2917,
    26051, 10275, 28310, 25897, 14277, 5242,  5698,  5617,  9129,  27250, 1372,  5396,  31126, 8303,  29941, 15439,
    9628,  21573, 18302, 17980, 11673, 18290, 11394, 11008};
static int a_ref[N] = {
    8,     20,    28,    42,    72,    140,   171,   227,   296,   331,   335,   347,   356,   387,   457,   459,
    543,   597,   599,   619,   741,   760,   791,   797,   877,   897,   937,   1055,  1073,  1118,  1198,  1372,
    1480,  1498,  1520,  1524,  1547,  1583,  1593,  1618,  1658,  1846,  1906,  1921,  1923,  1950,  1952,  1976,
    1977,  1984,  2049,  2064,  2079,  2086,  2113,  2137,  2151,  2202,  2212,  2213,  2223,  2249,  2315,  2318,
    2368,  2412,  2543,  2544,  2582,  2603,  2736,  2747,  2749,  2760,  2811,  2813,  2842,  2849,  2864,  2867,
    2917,  2935,  2980,  3064,  3079,  3145,  3185,  3191,  3234,  3237,  3264,  3270,  3301,  3305,  3305,  3340,
    3394,  3396,  3426,  3499,  3575,  3615,  3681,  3681,  3701,  3712,  3758,  3792,  3801,  3902,  3907,  3953,
    3954,  4015,  4015,  4048,  4121,  4153,  4160,  4165,  4174,  4202,  4217,  4218,  4221,  4260,  4274,  4284,
    4298,  4300,  4301,  4304,  4337,  4344,  4378,  4438,  4464,  4475,  4647,  4692,  4705,  4713,  4724,  4768,
    4863,  4920,  4945,  4959,  4981,  5022,  5051,  5098,  5101,  5130,  5216,  5235,  5242,  5274,  5301,  5331,
    5363,  5373,  5396,  5452,  5463,  5468,  5493,  5511,  5602,  5617,  5620,  5624,  5627,  5672,  5698,  5702,
    5854,  5890,  5899,  5961,  5963,  6012,  6054,  6071,  6078,  6084,  6106,  6111,  6111,  6131,  6165,  6180,
    6184,  6214,  6243,  6264,  6384,  6403,  6426,  6492,  6498,  6605,  6613,  6649,  6690,  6773,  6782,  6808,
    6813,  6836,  6848,  6910,  7008,  7019,  7052,  7090,  7128,  7177,  7179,  7218,  7218,  7285,  7305,  7372,
    7383,  7429,  7444,  7494,  7556,  7565,  7565,  7570,  7599,  7626,  7629,  7631,  7710,  7759,  7770,  7875,
    7884,  7896,  7917,  8133,  8276,  8303,  8365,  8405,  8446,  8534,  8549,  8557,  8569,  8659,  8734,  8740,
    8758,  8759,  8792,  8812,  8813,  8821,  8881,  8950,  8980,  9029,  9129,  9170,  9173,  9279,  9282,  9284,
    9303,  9320,  9388,  9406,  9418,  9418,  9452,  9475,  9476,  9486,  9488,  9530,  9552,  9563,  9568,  9571,
    9628,  9635,  9709,  9744,  9786,  9790,  9796,  9821,  9895,  9899,  9933,  9988,  10022, 10042, 10047, 10060,
    10169, 10172, 10275, 10283, 10298, 10331, 10428, 10474, 10506, 10588, 10617, 10629, 10630, 10655, 10679, 10724,
    10765, 10797, 10802, 10804, 10812, 10843, 11008, 11101, 11134, 11143, 11145, 11213, 11253, 11325, 11332, 11335,
    11351, 11394, 11406, 11411, 11485, 11509, 11544, 11557, 11574, 11621, 11643, 11673, 11738, 11791, 11796, 11819,
    11935, 12029, 12056, 12060, 12074, 12084, 12114, 12128, 12137, 12153, 12153, 12181, 12216, 12266, 12310, 12351,
    12463, 12516, 12619, 12644, 12653, 12653, 12691, 12695, 12790, 12794, 12827, 12849, 12863, 12909, 12919, 12928,
    12988, 13036, 13038, 13052, 13078, 13172, 13210, 13376, 13435, 13590, 13640, 13647, 13697, 13737, 13753, 13759,
    13782, 13785, 13997, 14036, 14049, 14089, 14119, 14128, 14132, 14206, 14226, 14277, 14277, 14293, 14317, 14373,
    14435, 14492, 14500, 14524, 14526, 14540, 14552, 14573, 14580, 14610, 14673, 14748, 14817, 14827, 14827, 14828,
    14849, 14882, 14893, 14909, 14952, 14960, 15002, 15049, 15102, 15107, 15219, 15223, 15285, 15317, 15353, 15363,
    15439, 15455, 15537, 15571, 15583, 15598, 15650, 15676, 15710, 15717, 15744, 15801, 15803, 15831, 15888, 15923,
    15940, 15950, 15979, 15992, 16010, 16015, 16046, 16069, 16073, 16088, 16126, 16178, 16192, 16310, 16311, 16362,
    16378, 16416, 16446, 16473, 16481, 16612, 16624, 16647, 16696, 16750, 16781, 16784, 16812, 16818, 16832, 16928,
    17034, 17050, 17078, 17119, 17151, 17255, 17319, 17352, 17392, 17434, 17470, 17498, 17530, 17535, 17543, 17550,
    17567, 17581, 17590, 17591, 17635, 17656, 17665, 17685, 17756, 17820, 17868, 17907, 17937, 17980, 18021, 18048,
    18077, 18118, 18161, 18174, 18182, 18211, 18222, 18266, 18285, 18290, 18302, 18331, 18361, 18389, 18401, 18465,
    18518, 18523, 18559, 18562, 18616, 18620, 18637, 18639, 18690, 18715, 18781, 18782, 18826, 18832, 18840, 18887,
    18963, 19060, 19075, 19076, 19086, 19087, 19100, 19132, 19151, 19165, 19172, 19190, 19217, 19233, 19249, 19326,
    19337, 19380, 19402, 19404, 19463, 19489, 19499, 19535, 19567, 19593, 19599, 19630, 19631, 19678, 19714, 19773,
    19840, 19865, 19900, 19961, 19980, 20072, 20135, 20254, 20262, 20314, 20315, 20371, 20386, 20390, 20402, 20449,
    20498, 20550, 20555, 20597, 20606, 20611, 20621, 20642, 20662, 20667, 20718, 20735, 20762, 20772, 20784, 20789,
    20888, 20900, 20922, 20949, 20989, 20993, 21026, 21066, 21102, 21152, 21158, 21166, 21167, 21184, 21295, 21307,
    21313, 21324, 21332, 21359, 21464, 21553, 21572, 21573, 21575, 21648, 21651, 21686, 21700, 21817, 21843, 21888,
    21929, 21943, 21952, 21965, 22000, 22027, 22057, 22268, 22275, 22331, 22378, 22387, 22418, 22471, 22472, 22490,
    22527, 22530, 22554, 22587, 22616, 22618, 22643, 22681, 22706, 22727, 22763, 22864, 22910, 22921, 23005, 23018,
    23079, 23256, 23265, 23302, 23314, 23316, 23329, 23355, 23357, 23370, 23379, 23414, 23427, 23450, 23461, 23470,
    23521, 23563, 23623, 23679, 23730, 23735, 23737, 23770, 23772, 23772, 23777, 23782, 23792, 23820, 23837, 23843,
    23936, 23995, 24006, 24037, 24076, 24110, 24111, 24128, 24151, 24167, 24196, 24223, 24236, 24257, 24304, 24342,
    24352, 24357, 24371, 24374, 24399, 24437, 24464, 24508, 24535, 24626, 24726, 24750, 24816, 24844, 24846, 24865,
    24896, 24916, 24971, 25073, 25161, 25229, 25251, 25274, 25304, 25331, 25342, 25346, 25397, 25414, 25432, 25455,
    25458, 25613, 25661, 25687, 25750, 25766, 25773, 25780, 25784, 25822, 25848, 25859, 25897, 25911, 25942, 26004,
    26014, 26014, 26032, 26051, 26071, 26102, 26104, 26132, 26187, 26236, 26258, 26263, 26285, 26312, 26380, 26416,
    26420, 26439, 26474, 26512, 26554, 26555, 26588, 26590, 26597, 26599, 26618, 26645, 26714, 26863, 26895, 26933,
    26948, 26974, 27051, 27068, 27142, 27218, 27220, 27250, 27273, 27290, 27309, 27314, 27323, 27343, 27344, 27352,
    27367, 27368, 27392, 27399, 27446, 27467, 27469, 27498, 27532, 27549, 27549, 27570, 27607, 27647, 27660, 27687,
    27717, 27774, 27793, 27823, 27830, 27835, 27848, 27888, 27908, 27934, 27952, 27977, 28002, 28009, 28025, 28039,
    28063, 28159, 28161, 28193, 28268, 28310, 28320, 28333, 28356, 28384, 28387, 28430, 28447, 28464, 28466, 28501,
    28518, 28521, 28522, 28565, 28587, 28598, 28696, 28701, 28703, 28713, 28744, 28773, 28774, 28786, 28800, 28865,
    28980, 29026, 29043, 29052, 29054, 29079, 29089, 29115, 29158, 29231, 29237, 29259, 29261, 29276, 29293, 29441,
    29484, 29485, 29542, 29547, 29558, 29598, 29778, 29891, 29907, 29913, 29927, 29941, 30002, 30010, 30045, 30068,
    30101, 30202, 30240, 30279, 30282, 30347, 30388, 30505, 30507, 30534, 30565, 30579, 30669, 30780, 30818, 30842,
    30907, 30914, 30934, 31013, 31029, 31081, 31100, 31126, 31201, 31222, 31256, 31302, 31329, 31374, 31377, 31382,
    31432, 31463, 31477, 31493, 31537, 31551, 31581, 31608, 31622, 31624, 31632, 31653, 31710, 31886, 31903, 31918,
    31936, 31946, 32083, 32101, 32162, 32209, 32211, 32283, 32295, 32400, 32416, 32438, 32454, 32457, 32479, 32551,
    32616, 32617, 32659, 32663, 32664, 32670, 32726, 32754};

void shell6(void) {
    int i, j, err;
    int* result;

    err = 0;
    printf("quick sort test begin.\n");
    for (i = 0; i < 10; i++) {
        printf("iteration %d begin\n", i);
        result = quick_sort(a, N);
        for (j = 0; j < N; j++) {
            if (result[j] != a_ref[j]) {
                err += 1;
                printf("iteration %d errored\n", i);
                break;
            }
        }
        printf("iteration %d end\n", i);
    }

    if (err == 0) {
        printf("quick sort PASS!\n");
    } else {
        printf("quick sort ERROR!!!\n");
    }

    return;
}

__attribute__((section(".init"))) void _start() {
    shell6();
    exit();
    return;
}
