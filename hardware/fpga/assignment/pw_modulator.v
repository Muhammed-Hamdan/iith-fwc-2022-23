module pw_modulator(
	input reset,
	input X,
	output reg fsm_clk,
	output reg Z
);

//declaring variables
reg	[26:0] 	delay = 0;
reg	[3:0] 	state_current = 0;
wire 		clk;
wire		A;
wire		B;
wire		C;
wire		D;

qlal4s3b_cell_macro u_qlal4s3b_cell_macro (
	.Sys_Clk0 (clk),
);

always@(posedge clk or posedge reset)
begin
	if(reset)
		delay = 27'b0;

	else
	begin
		delay = delay + 1;
		if(delay > 20000000)
		begin
			delay = 27'b0;
			fsm_clk = !fsm_clk;
		end
	end
end

always@(posedge fsm_clk or posedge reset)
begin
	if(reset)
	begin
		state_current <= 4'b0;
		Z <= 1'b0;
	end

	else
	begin
		state_current[3] <= (B&!D) | (B&!C&!X) | (!B&C&D&!X);
		state_current[2] <= (!B&C&!D) | (!B&C&X) | (!A&!B&!C&!X) | (B&!C&D&X);
		state_current[1] <= (!B&D&X) | (!C&D&X) | (A&!D&!X) | (!A&!B&!C&X) | (!B&C&!D&!X);
		state_current[0] <= (!A&!B&X) | (!B&!D&X) | (B&!D&!X) | (B&C&!X) | (!A&!C&D&X);
		Z <= (C&!D) | (B&D) | (A&!D) | (A&X) | (!A&!C&D);
	end
end

assign {A,B,C,D} = state_current;

endmodule
