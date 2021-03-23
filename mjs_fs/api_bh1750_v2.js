let BH1750 = {
    init : ffi('int bh1750_init()'),
    lux : ffi('int bh1750_read()'),
};
