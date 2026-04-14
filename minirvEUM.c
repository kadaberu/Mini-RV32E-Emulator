#include<stdio.h>
#include<stdint.h>
uint32_t ebreak=0;
uint32_t PC=0;
uint32_t GPR[16];
uint8_t RAM[4*1048576];
uint8_t ROM[4*1048576];
uint32_t instruction;
uint32_t opcode_0;
uint32_t opcode_1;
uint32_t imm_12;
uint32_t rs1;
uint32_t funct3;
uint32_t rd;
uint32_t rs2;
uint32_t imm_20;
uint32_t imm_7;
uint32_t addi=0;
uint32_t jalr=0;
uint32_t add=0;
uint32_t lui=0;
uint32_t lw=0;
uint32_t lbu=0;
uint32_t sw=0;
uint32_t sb=0;
uint32_t store_address;
uint32_t load_address;
void opcode_decoder(){
    addi=0;jalr=0;add=0;lui=0; 
    lw=0;lbu=0;sw=0;sb=0;ebreak=0;
    opcode_0=instruction&0x7F;
    opcode_1=(instruction>>12)&0x7;
    if(opcode_0==0x13){
        addi=1;
    }
    if(opcode_0==0x67){
        jalr=1;
    }
    if(opcode_0==0x33&&opcode_1==0x0){
        add=1;
    }
    if(opcode_0==0x37){
        lui=1;
    }
    if(opcode_0==0x3&&opcode_1==0x2){
        lw=1;
    }
    if(opcode_0==0x3&&opcode_1==0x4){
        lbu=1;
    }
    if(opcode_0==0x23&&opcode_1==0x2){                        
        sw=1;
    }
    if(opcode_0==0x23&&opcode_1==0x0){
        sb=1;
    }
    if(instruction==0x00100073){
        ebreak = 1;
    }
}
void field_extraction(){
    imm_12=(instruction>>20)&0xFFF;
    rs1=(instruction>>15)&0x0F;
    funct3=(instruction>>12)&0x7;
    rd=(instruction>>7)&0x0F;
    rs2=(instruction>>20)&0x0F;
    imm_20=(instruction>>12)&0xFFFFF;
    imm_7=(instruction>>25)&0x7F;
}
void inst_cycle(){
    uint32_t load_bit;
    uint32_t store_bit;
    uint32_t next_pc=PC+4;
    if(addi==1){
        GPR[rd]=GPR[rs1]+((int32_t)(imm_12 << 20)>>20);
   
    }
    if(add==1){
        GPR[rd]=GPR[rs1]+GPR[rs2];
        
    }
    if(lui==1){
        GPR[rd]=imm_20<<12;
   
    }
    if(lw==1){
        load_address=GPR[rs1]+((int32_t)(imm_12<<20)>>20);
        GPR[rd]=RAM[load_address]|(RAM[load_address+1]<<8)|(RAM[load_address+2]<<16)|(RAM[load_address+3]<<24);
      
    }
    if(lbu==1){
    load_address=GPR[rs1]+((int32_t)(imm_12<<20)>>20);
    GPR[rd] = RAM[load_address];
   
    }
    if(sw==1){
        store_address=((int32_t)((((instruction>>25)<<5)|((instruction>>7)&0x1F))<<20)>>20)+GPR[rs1];
        RAM[store_address]=GPR[rs2]&0xFF;
        RAM[store_address+1]=(GPR[rs2]>>8)&0xFF;
        RAM[store_address+2]=(GPR[rs2]>>16)&0xFF;
        RAM[store_address+3]=(GPR[rs2]>>24)&0xFF;
   
    }
    if(sb==1){
    store_address=((int32_t)((((instruction>>25)<<5)|((instruction>>7)&0x1F))<<20)>>20)+GPR[rs1];
    RAM[store_address] = GPR[rs2] & 0xFF;

}
if(jalr==1){
    uint32_t target=(GPR[rs1]+((int32_t)(imm_12<<20)>>20))&~1;
    if(rd!=0) GPR[rd]=PC+4;
    next_pc=target;
}
    PC=next_pc;
    GPR[0]=0;
}
int main(){
    int i=0;
  for(i=0;i<16;i++){
    GPR[i]=0;
  }
  for(int i=0;i<4*1048576;i++){
        ROM[i]=0; 
    }
  FILE *fp_rom=fopen("mem.bin","rb");
  if(fp_rom==NULL){
        printf("Error\n");
        return -1;
    }
    fread(ROM,1,4*1048576,fp_rom);
    ROM[0x224]=0x73;
    ROM[0x224+1]=0x00;
    ROM[0x224+2]=0x10;
    ROM[0x224+3]=0x00;
        fclose(fp_rom);
  FILE *fp_ram=fopen("mem.bin","rb");
  if(fp_ram==NULL){
        printf("Error\n");
        return -1;
    }
    fread(RAM,1,4*1048576,fp_ram);
        fclose(fp_ram);
        while(PC/4<1048576){
            instruction=ROM[PC]|(ROM[PC+1]<<8)|(ROM[PC+2]<<16)|(ROM[PC+3]<<24);
          
            opcode_decoder();
            field_extraction();
            inst_cycle();
            if(ebreak==1){
           if(GPR[10]==0){
            printf("HIT GOOD TRAP\n");
           }
            if(GPR[10]!=0){
                printf("HIT BAD TRAP\n");
            }
           break;
        }


        }
        return 0;
    }