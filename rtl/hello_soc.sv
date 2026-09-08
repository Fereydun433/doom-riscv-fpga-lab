module hello_soc (
    input  logic        clk,
    input  logic        resetn,

    output wire         trap,

    output logic        console_valid,
    output logic [7:0]  console_data,

    output logic        done,
    output logic [31:0] exit_code,

    output logic        bus_fault,
    output logic [31:0] fault_addr,

    output wire         mem_valid,
    output wire         mem_instr,
    output logic        mem_ready,
    output wire [31:0]  mem_addr,
    output wire [31:0]  mem_wdata,
    output wire [3:0]   mem_wstrb,
    output logic [31:0] mem_rdata
);

    localparam logic [31:0] RAM_BYTES =
        32'h0001_0000;

    localparam logic [31:0] CONSOLE_ADDR =
        32'h1000_0000;

    localparam logic [31:0] EXIT_ADDR =
        32'h1000_0004;

    logic [31:0] ram [0:16383];

    string firmware_path;

    initial begin
        if (!$value$plusargs("firmware=%s", firmware_path))
            firmware_path = "build/riscv/hello.hex";

        $readmemh(firmware_path, ram);
    end

    picorv32 #(
        .PROGADDR_RESET(32'h0000_0000),
        .STACKADDR(32'h0001_0000),
        .ENABLE_COUNTERS(0),
        .COMPRESSED_ISA(0),
        .ENABLE_MUL(0),
        .ENABLE_DIV(0),
        .ENABLE_IRQ(0),
        .CATCH_MISALIGN(1),
        .CATCH_ILLINSN(1)
    ) cpu (
        .clk(clk),
        .resetn(resetn),
        .trap(trap),

        .mem_valid(mem_valid),
        .mem_instr(mem_instr),
        .mem_ready(mem_ready),
        .mem_addr(mem_addr),
        .mem_wdata(mem_wdata),
        .mem_wstrb(mem_wstrb),
        .mem_rdata(mem_rdata),

        .mem_la_read(),
        .mem_la_write(),
        .mem_la_addr(),
        .mem_la_wdata(),
        .mem_la_wstrb(),

        .pcpi_valid(),
        .pcpi_insn(),
        .pcpi_rs1(),
        .pcpi_rs2(),

        .pcpi_wr(1'b0),
        .pcpi_rd(32'b0),
        .pcpi_wait(1'b0),
        .pcpi_ready(1'b0),

        .irq(32'b0),
        .eoi(),
        .trace_valid(),
        .trace_data()
    );

    always_ff @(posedge clk) begin
        mem_ready <= 1'b0;
        console_valid <= 1'b0;

        if (!resetn) begin
            mem_rdata <= 32'b0;
            console_data <= 8'b0;
            done <= 1'b0;
            exit_code <= 32'b0;
            bus_fault <= 1'b0;
            fault_addr <= 32'b0;
        end
        else if (mem_valid && !mem_ready && !done && !bus_fault) begin
            mem_ready <= 1'b1;
            mem_rdata <= 32'b0;

            if (mem_addr < RAM_BYTES) begin
                mem_rdata <= ram[mem_addr[15:2]];

                for (int lane = 0; lane < 4; lane++) begin
                    if (mem_wstrb[lane])
                        ram[mem_addr[15:2]][lane*8 +: 8]
                            <= mem_wdata[lane*8 +: 8];
                end
            end
            else if (!mem_instr &&
                     mem_addr == CONSOLE_ADDR &&
                     (mem_wstrb == 4'b0001 ||
                      mem_wstrb == 4'b1111)) begin
                console_data <= mem_wdata[7:0];
                console_valid <= 1'b1;
            end
            else if (!mem_instr &&
                     mem_addr == EXIT_ADDR &&
                     mem_wstrb == 4'b1111) begin
                exit_code <= mem_wdata;
                done <= 1'b1;
            end
            else begin
                bus_fault <= 1'b1;
                fault_addr <= mem_addr;
            end
        end
    end

endmodule
