#include "daqboard1.h"

DaqBoard1::DaqBoard1(QObject *parent) :
    board(parent)
{
    name = "DaqBoard1";
    mSerial.setBaudRate(1000000);
    reset();

    inputs.insert(inputs.end(), "ENC1");
    inputs.insert(inputs.end(), "ENC2");

    outputs.insert(outputs.end(), "DAC1");
    outputs.insert(outputs.end(), "DAC2");

    connect(&mSerial, SIGNAL(readyRead()), SLOT(on_serial_read()));
}
void DaqBoard1::init()
{
    //do nothing
}
void DaqBoard1::start()
{
    if(!mSerial.open(QIODevice::ReadWrite)){
        cout << "[TARGET] Couldn't open serial port!";
    }
}
void DaqBoard1::stop()
{
    dac.dac1 = 2048;
    dac.dac2 = 2048;
    syncOutputs();
    mSerial.close();
}
void DaqBoard1::pause()
{
    dac.dac1 = 2048;
    dac.dac2 = 2048;
    syncOutputs();
}
void DaqBoard1::resume()
{
    //do nothing
}
void DaqBoard1::reset()
{
    enc.enc1 = 0;
    enc.enc2 = 0;
    dac.dac1 = 2048;
    dac.dac2 = 2048;
    mSerialBuf.clear();
}
int DaqBoard1::enableInput(QString name)
{
    if(name == "ENC1") return 1;
    if(name == "ENC2") return 2;
    return -1;
}
int DaqBoard1::enableOutput(QString name)
{
    if(name == "DAC1") return 3;
    if(name == "DAC2") return 4;
    return -1;
}
double DaqBoard1::getInput(int id)
{
    if(id == 1){
        return (double)(enc.enc1)*2.0*pi/4096;
    }
    else if (id == 2) {
        return (double)(enc.enc2)*1.0*pi/4096;
    }
}
void DaqBoard1::setOutput(int id, double value)
{
    if(id == 3){
        dac.dac1 = (uint16_t)(value*2048.0/10.0+2048.0);
    }
    else if (id == 4) {
        dac.dac2 = (uint16_t)(value*2048.0/10.0+2048.0);
    }
}
void DaqBoard1::syncOutputs()
{
    mSerial.write((char*)&dac, sizeof(dac_msg_t));
    mSerial.flush();
}
void DaqBoard1::openSettingsDialog()
{
    //do nothing
}
void DaqBoard1::on_serial_read(){
    mSerialBuf.append(mSerial.readAll());
    while(mSerialBuf.size() >= sizeof (enc_msg_t)){
        if(mSerialBuf.size() == sizeof (enc_msg_t)){
            memcpy(&enc, mSerialBuf.data(), sizeof(enc_msg_t));
            mSerialBuf.remove(0,sizeof (enc_msg_t));
        }
        else {
            mSerialBuf.remove(0,1);
        }
    }
}