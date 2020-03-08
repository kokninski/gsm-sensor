
pin_array = [
    "PA_8",
    "PA_9",  # RXD
    "PA_10", # TXD
    "PA_11", # USB D-
    "PA_12", # USB D+
    "PA_13", # JTDI
    "PA_14", # JTCK
    "PA_15",
    "PB_3",  # JTDO
    "PB_4",  # JTRST
    "PB_5",
    "PB_6",
    "PB_7",
    "PB_8",
    "PB_9",
    "PC_13",
    "PC_14",   # PC_14 OSC32_1
    "PC_15",   # PC_15 OSC32_2
    "PH_1",    # PD_0 OSCIN
    "PH_2",    # PD_1 OSCOUT
    "PB_2",
    "PB_10",
    "PB_11",

    "PA_0",  # A0
    "PA_1",  # A1
    "PA_2",  # A2
    "PA_3",  # A3
    "PA_4",  # A4
    "PA_5",  # A5
    "PA_6",  # A6
    "PA_7",  # A7
    "PB_0",  # A8
    "PB_1",  # A9
    "PB_12",  # A10
    "PB_13",  # A11
    "PB_14",  # A12  #LED
    "PB_15"   # A13  #LED
]

for num, item in enumerate(pin_array):
    port, pin = item.split("_")
    pin_name = port+pin
    print(f"#define {pin_name} {num}")
