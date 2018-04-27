`timescale 1ns / 1ps

module accelerator(
    input           clk,
    input           resetn,
    input           start,
    input  [1023:0] din,
    output [1023:0] dout,
    output          done);
    
    ////////////// - State Machine

    reg [3:0] state, next_state;
    
    localparam STATE_IDLE    = 2'b00;
    localparam STATE_COMPUTE = 2'b01;
    
    always @(*)
    begin
        if (resetn==1'b0)
            next_state <= STATE_IDLE;
        else
        begin
            case (state)
                STATE_IDLE:
                    if (start) next_state <= STATE_COMPUTE;
                    else       next_state <= STATE_IDLE;
                
                STATE_COMPUTE:
                    // in this simple example, the computation is single cycle, 
                    // so we don't need to wait a condition for its completion
                    // therefore next_state is already known 
                    next_state <= STATE_IDLE;
                
                default:
                    next_state <= STATE_IDLE;
            endcase
        end
    end
    
    // State Update

    always @(posedge(clk))
        if (resetn==1'b0)
            state <= STATE_IDLE;
        else
            state <= next_state;    

    ////////////// - Computation



    // Create the data register

    // The register itself
    reg [1023:0] reg_data;
    
    // The register's control signals
    reg          reg_cmd;
    reg          reg_en;

    always @(posedge(clk))
        if (reg_en) begin
            if (reg_cmd)    reg_data <= {32'hDEADBEEF,{reg_data[991:0]}};
            else            reg_data <= din;
        end
        // if reg_en = 0, keep the value as it was            

    assign dout = reg_data;



    // Create the done register

    // The register itself
    reg reg_done;
    
    // The register's control signals
    reg reg_done_en;
    reg reg_done_reset;

    always @(posedge(clk))
             if (reg_done_reset)    reg_done = 1'b0;
        else if (reg_done_en   )    reg_done = 1'b1;
        else                        reg_done = reg_done;
    
    assign done = reg_done;



    // Control the registers by the use of their control signals

    always @(*)
        if (resetn==1'b0) begin
            reg_done_reset <= 1'b1;
            reg_done_en    <= 1'b0;

            reg_en         <= 1'b0;  // Disable register
            reg_cmd        <= 1'b0;  // Register is disabled, so command is dummy
            
        end 
        else begin
            case (state)
                STATE_IDLE: begin
                    if (start)  begin 
                        reg_done_reset  <= 1'b0;
                        reg_done_en     <= 1'b0;
                        
                        reg_en          <= 1'b1;    // Enable register
                        reg_cmd         <= 1'b0;    // Command to fetch din                        
                    end                    
                    else begin 
                        reg_done_reset  <= 1'b0;
                        reg_done_en     <= 1'b0;
                        
                        reg_en          <= 1'b0;    // Disable register
                        reg_cmd         <= 1'b0;    // Register is disabled, so command is dummy
                    end
                end

                STATE_COMPUTE: begin
                    reg_done_reset  <= 1'b0;
                    reg_done_en     <= 1'b1;

                    reg_en          <= 1'b1;    // Enable register
                    reg_cmd         <= 1'b1;    // Command to XOR
                    
                end
                
                default: begin
                    reg_done_reset  <= 1'b1;
                    reg_done_en     <= 1'b0;
    
                    reg_en          <= 1'b0;    // Enable register
                    reg_cmd         <= 1'b0;    // Command to XOR
                end
                    
            endcase
        end


endmodule