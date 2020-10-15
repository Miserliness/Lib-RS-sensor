#include "radSens1v2.h"

ClimateGuard_RadSens1v2::ClimateGuard_RadSens1v2(uint8_t sensor_address)
{
    _sensor_address = sensor_address;
}

ClimateGuard_RadSens1v2::~ClimateGuard_RadSens1v2()
{
}

bool ClimateGuard_RadSens1v2::radSens_init()
{
    Wire.begin();
    Wire.beginTransmission(_sensor_address); //safety check, make sure the sensor is connected
    Wire.write(0x0);
    if (Wire.endTransmission(true) != 0)
        return false;
    updateData();
    return true;
}

bool ClimateGuard_RadSens1v2::updateData()
{
    Wire.requestFrom(_sensor_address, 19);
    for (int i = 0; i < 19; i++)
    {
        _data[i] = Wire.read();
    }
    if (_data[0] == 0x7D)
    {
	_pulse_cnt += (_data[9] << 8) | _data[10];
        return true;
    }
    return false;
}

uint8_t ClimateGuard_RadSens1v2::getChipId()
{
    _chip_id = _data[0];
    return _chip_id;
}

uint8_t ClimateGuard_RadSens1v2::getFirmwareVersion()
{
    _firmware_ver = _data[1];
    return _firmware_ver;
}

float ClimateGuard_RadSens1v2::getRadIntensyDyanmic()
{
    if (updateData())
    {
        float temp = ((_data[3] << 16) | (_data[4] << 8) | _data[5]) / 10.0;
        return temp;
    }
    else
    {
        return 0;
    }
}

float ClimateGuard_RadSens1v2::getRadIntensyStatic()
{
    if (updateData())
    {
        float temp = ((_data[6] << 16) | (_data[7] << 8) | _data[8]) / 10.0;
        return temp;
    }
    else
    {
        return 0;
    }
}

uint32_t ClimateGuard_RadSens1v2::getNumberOfPulses()
{
    if (updateData())
    {
        return _pulse_cnt;
    }
    else
    {
        return 0;
    }
}

uint8_t ClimateGuard_RadSens1v2::getSensorAddress()
{
    if (updateData())
    {
        _sensor_address = _data[16];
        return _sensor_address;
    }
    return 0;
}

bool ClimateGuard_RadSens1v2::getHVGeneratorState()
{
    if (updateData())
    {
        return _data[17];
    }
    return 0;
}

uint8_t ClimateGuard_RadSens1v2::getSensitivity()
{
    if (updateData())
    {
        return _data[18];
    }
    return 0;
}

bool ClimateGuard_RadSens1v2::setHVGeneratorState(bool state)
{
    Wire.beginTransmission(0x66);
    Wire.write(0x11);    
    if (state) {
        Wire.write(1);
    } else {
        Wire.write(0);  
    }
    if (Wire.endTransmission(true) == 0) return true;  //"true" sends stop message after transmission & releases I2C bus
    return false;
}

bool ClimateGuard_RadSens1v2::setSensitivity(uint8_t sens)
{
    Wire.beginTransmission(0x66);
    Wire.write(0x12);    
    Wire.write(sens);
    if (Wire.endTransmission(true) == 0) return true; 
    return false;
}