module down_counter(
	// Inputs
	input wire [7:0] in_data,
	input wire latch,
	input wire dec,
	input wire div,
	// Outputs
	output reg [7:0] count,
	output reg clk,
	output wire zero
);

// Internal
reg	[26:0] 	delay = 0;
wire sys_clk;

qlal4s3b_cell_macro u_qlal4s3b_cell_macro (
	.Sys_Clk0 (sys_clk),
);

always@(posedge sys_clk)
begin
	delay = delay + 1;
	if(delay > 20000000)
	begin
		delay = 27'b0;
		clk = !clk;
	end
end

always @(posedge clk)
begin
	if(latch)
		count <= in_data;
	else if(dec)
		count <= (count==8'd0) ? 8'd0 : count-1;
	else if(div)
		count <= count >> 1;
end

assign zero = (count == 0);

endmodule
