// map for qmr files
/*
after Rule number
    type: input type - output type (e.g., note, controller, pitchbend, prog chng 0123)
        0 = Note
        1 = Controller (Control Change)
        2 = Pitch Bend
        3 = Program Change
    chan: min - max (0 - 15) - out offset
    $type: min - max  (0 - 127) - out offset
        - For Note (0): Represents the note number.
        - For Controller (1): Represents the CC number.
        - For Pitch Bend (2): Obsolete (not used).
        - For Program Change (3): Represents the offset, reverse, or fixed value.
    value: min - max (0 - 127) - out offset min - max
        - For Note (0): Represents the velocity.
        - For Controller (1): Represents the CC value.
        - For Pitch Bend (2): Represents the pitch bend range and scales the value.
        - For Program Change (3): Obsolete (not used).
    placeholders:
        1st: channel behavior (0 = offset, 1 = reverse offset, 2 = fixed value)
        2nd: value behavior (0 = offset, 1 = reverse offset, 2 = fixed value)
    out port: (0/1) - single value
*/