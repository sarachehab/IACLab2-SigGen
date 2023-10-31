#include "Vsinegen.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp"

#define MAX_SIM_CYC 1000000

int main(int argc, char **argv, char **env)
{
    int tick;
    int simcyc;

    Verilated::commandArgs(argc, argv);

    // init top verilog instance, sinegen
    Vsinegen *top = new Vsinegen;

    // init trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC *tfp = new VerilatedVcdC;
    top->trace(tfp, 99);
    tfp->open("sinegen.vcd");

    // init Vbuddy
    if (vbdOpen() != 1)
        return (-1);
    vbdHeader("Lab 2: Sine Generator");

    // init simulation inputs
    top->clk = 1;
    top->rst = 0;
    top->en = 1;
    top->incr = 1;
    top->offset = vbdValue();

    // run simulation for N samples
    for (simcyc = 0; simcyc < MAX_SIM_CYC; simcyc++)
    {

        for (tick = 0; tick < 2; tick++)
        {
            tfp->dump(2 * simcyc + tick);
            top->clk = !top->clk;
            top->eval();
        }

        // ++++ Send count value to Vbuddy
        vbdPlot(int(top->dout1), 0, 256);
        vbdPlot(int(top->dout2), 0, 256);
        vbdCycle(simcyc);

        // modify signal frequency
        top->offset = vbdValue();

        if ((Verilated::gotFinish()) || (vbdGetkey() == 'q'))
        {
            exit(0);
        }
    }

    vbdClose();
    tfp->close();
    exit(0);
}