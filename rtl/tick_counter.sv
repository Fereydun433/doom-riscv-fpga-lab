// SPDX-License-Identifier: GPL-2.0-only
// First learning exercise: synchronous reset, enable, and 8-bit rollover.
module tick_counter (
    input  logic       clk,
    input  logic       rst,
    input  logic       enable,
    output logic [7:0] count
);
    always_ff @(posedge clk) begin
        if (rst)
            count <= 8'd0;
        else if (enable)
            count <= count + 8'd1;
    end
endmodule
