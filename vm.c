#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "unistd.h"


#include <netdb.h>
#include <netinet/in.h>

#include <string.h>

#define VIRTMEM_SIZEMAX 0x400
#define CODE_SIZEMAX 256

typedef struct vm_t {
  uint8_t* code;
  size_t code_len;
  uint64_t* memory;
  uint64_t* regs;
}vm_t;


typedef enum registers_t{
  R_BAD,
  PC,
  SP,
  R0,
  R1,
  R2,
  R3,
  R4,
  R5,
  R6,
  R7,
  R_MAX
}registers_t;

typedef enum instructions_t{
  I_BAD = 0,
  XOR,
  PUSH,
  POP,
  MOV_L,
  MOV_H,
  ADD,
  SUB,
  CALL,
  MAX_INST
}instructions_t;

typedef enum func_call_t{
  F_BAD = 0,
  OPEN,
  READ,
  WRITE
}func_call_t;


uint8_t* get_pInstruction(vm_t* vm);
void vm_init(vm_t* vm);  
void vm_run(vm_t* vm);
void check_reg(uint8_t reg);
void do_XOR(vm_t* vm);
void do_PUSH(vm_t* vm);
void do_POP(vm_t* vm);
void do_MOV_L(vm_t* vm);
void do_MOV_H(vm_t* vm);
void do_ADD(vm_t* vm);
void do_SUB(vm_t* vm);
void do_CALL(vm_t* vm);


void check_sp(vm_t* vm);
void check_pc(vm_t* vm);

void debug(char* s)
{
#if 1
  printf(s);
#endif
  
}


  
int main(int argc, char** argv)
{

  vm_t* vm = (vm_t*)malloc(sizeof(vm_t));
  vm_init(vm);
  uint64_t i;

  
  if (0)
    {
      // Check XOR
      vm->code[0] = XOR;
      vm->code[1] = R0;
      vm->code[2] = R0;
      vm->code[3] = I_BAD;
      vm->code_len = 3;
      
      printf("R0 : %d\n", vm->regs[R0]);
      vm_run(vm);
      printf("R0 : %d\n", vm->regs[R0]);
    }
  if (0)
    {
      // PUSH
      vm->code[0] = PUSH;
      vm->code[1] = R0;
      vm->code_len = 2;
      printf("Memory : %p,  Memory value %d, SP : %p\n",
	     vm->memory,
	     *vm->memory,
	     vm->regs[SP]);
      vm_run(vm);
      printf("Memory : %p,  Memory value %d, SP : %p\n",
	     vm->memory,
	     *vm->memory,
	     vm->regs[SP]);

    }
  if (0)
    {
      // POP
      *(vm->memory) = 0x41;
      vm->code[0] = POP;
      vm->code[1] = R0;
      vm->code_len = 2;
      printf("R0 : %d,  Memory value %d, SP : %p\n", vm->regs[R0], *vm->memory, vm->regs[SP]);
      vm_run(vm);
      printf("R0 : %d,  Memory value %d, SP : %p\n", vm->regs[R0], *vm->memory, vm->regs[SP]);

    }
  if (0)
    {
      // MOV 64bit into register
      vm->code[0] = MOV_L;
      vm->code[1] = R0;
      vm->code[2] = 0x41;
      vm->code[3] = 0x42;
      vm->code[4] = 0x43;
      vm->code[5] = 0x44;
      vm->code[6] = MOV_H;
      vm->code[7] = R0;
      vm->code[8] = 0x45;
      vm->code[9] = 0x46;
      vm->code[10] = 0x47;
      vm->code[11] = 0x48;
      vm->code_len = 12;
      printf("R0 : %llu,  Memory value %llu, SP : %llu\n",
	     vm->regs[R0],
	     *vm->memory,
	     vm->regs[SP]);
      vm_run(vm);
      printf("R0 : %lld,  Memory value %d, SP : %p\n",
	     (uint64_t)(vm->regs[R0]),
	     *vm->memory,
	     vm->regs[SP]);

    }
  if (0)
    {
      // ADD
      vm->code[0] = ADD;
      vm->code[1] = R0;
      vm->code[2] = R1;
      vm->code_len = 3;

      printf("R0 : %d, R1 : %d  Memory value %d, PC : %p\n",
	     vm->regs[R0],
	     vm->regs[R1],
	     *vm->memory,
	     vm->regs[PC]);
      
      vm_run(vm);
      printf("R0 : %d,  R1 : %d Memory value %d, PC : %p\n",
	     vm->regs[R0],
	     vm->regs[R1],
	     *vm->memory,
	     vm->regs[PC]);
    }
    
  if (0)
    {
      // SUB
      vm->code[0] = SUB;
      vm->code[1] = R0;
      vm->code[2] = R1;
      vm->code_len = 3;

      printf("R0 : %d, R1 : %d  Memory value %d, PC : %p\n",
	     vm->regs[R0],
	     vm->regs[R1],
	     *vm->memory,
	     vm->regs[PC]);
      
      vm_run(vm);
      printf("R0 : %d,  R1 : %d Memory value %d, PC : %p\n",
	     vm->regs[R0],
	     vm->regs[R1],
	     *vm->memory,
	     vm->regs[PC]);
    }
    if (0)
    {
      // CALL OPEN
      
      
      vm->code[0] = CALL;
      vm->code[1] = OPEN;
      vm->memory[0]= '\0';
      vm->memory[1]= 'B';
      // TODO
      
      vm->regs[SP] = (uint64_t)(vm->memory + 1);
      vm->code[2] = XOR;
      vm->code[3] = R3;
      vm->code[4] = R3;
      vm->code[5] = PUSH;
      vm->code[6] = R3;
      vm->code[7] = PUSH;
      vm->code[8] = R3;
      vm->code[9] = PUSH;
      vm->code[10] = R3;
      vm->code[11] = PUSH;
      vm->code[12] = R3;
      vm->code[13] = PUSH;
      vm->code[14] = R3;
      vm->code[15] = PUSH;
      vm->code[16] = R3;
      vm->code[17] = PUSH;
      vm->code[18] = R3;
      vm->code[19] = PUSH;
      vm->code[20] = R3;
      vm->code[21] = PUSH;
      vm->code[22] = R3;
      vm->code[23] = POP;
      vm->code[24] = R3;
      vm->code[25] = POP;
      vm->code[26] = R3;
      vm->code[27] = POP;
      vm->code[28] = R3;
      vm->code[29] = POP;
      vm->code[30] = R3;
      vm->code[31] = POP;
      vm->code[32] = R3;
      vm->code[33] = POP;
      vm->code[34] = R3;
      vm->code[35] = POP;
      vm->code[36] = R3;
      vm->code[37] = POP;
      vm->code[38] = R3;
      vm->code[39] = POP;
      vm->code[40] = R3;
      
      vm->code[41] = CALL;
      vm->code[42] = READ;
      vm->code[43] = R0;
      vm->code[44] = MOV_L;
      vm->code[45] = R5;
      vm->code[46] = 0x01;
      vm->code[47] = 0x00;
      vm->code[48] = 0x00;
      vm->code[49] = 0x00;
      
      
      vm->code[50] = CALL;
      vm->code[51] = WRITE;
      vm->code[52] = R5;

      vm->code_len = 53;


      vm_run(vm);
    }
    
    int sockfd, newsockfd, portno, clilen, reuse;
    char buffer[CODE_SIZEMAX];
    struct sockaddr_in serv_addr, cli_addr;
    int  n;
    
    /* First call to socket() function */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
   
    if (sockfd < 0) {
      perror("ERROR opening socket");
      exit(EXIT_FAILURE);
    }
    
    /* set SO_REUSEADDR and SO_REUSEPORT */
    reuse = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, (const char*)&reuse, sizeof(reuse)) < 0){
      perror("ERROR setsockopt for SO_REUSEADDR\n");
      exit(EXIT_FAILURE);
    }
    
    
    /* Initialize socket structure */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = 4242;
   
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
   
    /* Now bind the host address using bind() call.*/
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
      perror("ERROR on binding");
      exit(EXIT_FAILURE);
    }
      
    /* Now start listening for the clients, here process will
     * go in sleep mode and will wait for the incoming connection
     */
   
    if(listen(sockfd,10) < 0)
      {
	perror("Failed to listen\n");
	exit(EXIT_FAILURE);
      }

    
    


    for (;;)
      {
	clilen = sizeof(cli_addr);
	/* Accept actual connection from the client */
	newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
	/* Fork and check if child or parent */
	
	
	if (newsockfd < 0) {
	  perror("ERROR on accept");
	  exit(1);
	}
	if (fork() == 0)
	  {
	  /* child */

	
	  /* If connection is established then start communicating */
	  bzero(buffer,CODE_SIZEMAX);
	  n = read( newsockfd,&(vm->code_len), sizeof(uint8_t));   
	  
	  if (n < 0)
	    {
	      perror("ERROR reading from socket");
	      exit(1);
	    }
	  sleep(1);
	  printf("Code_len : %d\n", vm->code_len);
	  if ((vm->code_len >= CODE_SIZEMAX) || (vm->code_len <= 0))
	    {
	      printf("Bad code len\n");
	      exit(EXIT_FAILURE);
	    }
	  
	  n = read(newsockfd, vm->code, vm->code_len * sizeof(uint8_t));
	  if (n != (ssize_t)vm->code_len)
	    {
	      printf("Wrong len.\n");
	      exit(EXIT_FAILURE);
	    }
	  
	  if (n < 0)
	    {
	      perror("Error reading from socket");
	      exit(EXIT_FAILURE);
	    }
	  for (unsigned int t = 0; t < vm->code_len; t++)
	    {
	      //       printf("code : %d\n", (uint8_t)(vm->code +t); 
	    }

	  vm_run(vm);
	  
	  /* Write a response to the client */
	  n = write(newsockfd,"I got your message\n",19);
	  close(newsockfd);
	  
	  if (n < 0)
	    {
	      perror("ERROR writing to socket");
	      exit(1);
	    }
	  }
	else
	  {
	    /* parent, do nothing and close socket */
	    close(newsockfd);
	  }
      }
	    
	
  return 0;

}


void vm_init(vm_t* vm)
{
  vm->code = (uint8_t*)malloc(CODE_SIZEMAX * sizeof(uint8_t));
  if (!vm->code)
    {
      exit(EXIT_FAILURE);
    }
			   
  vm->memory = (uint64_t*)malloc(VIRTMEM_SIZEMAX * sizeof(uint64_t));
  if (!vm->memory)
    {
      exit(EXIT_FAILURE);
    }
  vm->code_len = 0;
  vm->regs = (uint64_t*)malloc(R_MAX * sizeof(uint64_t));
  vm->regs[PC] = (uint64_t)(vm->code);
  vm->regs[SP] = (uint64_t)(vm->memory);
  vm->regs[R0] = 0x70;
  vm->regs[R1] = 0x34;
  vm->regs[R2] = 0x73;
  vm->regs[R3] = 0x73;
  vm->regs[R4] = 0x77;
  vm->regs[R5] = 0x30;
  vm->regs[R6] = 0x72;
  vm->regs[R7] = 0x64;
}


void vm_run(vm_t* vm)
{
  uint8_t* pInst = get_pInstruction(vm);
  printf("VM RUN pInst : %llu vm->code %llu codelen : %llu \n", (unsigned long long)pInst, (unsigned long long)vm->code, (unsigned long long)vm->code_len );
  while(pInst - vm->code < vm->code_len && !(pInst < vm->code)){
    printf("PC:  %p\n", vm->regs[PC]);
    printf("pInst - vmcode = %d\n", pInst - vm->code);
    switch(*pInst)
      {
      case I_BAD:
	exit(EXIT_FAILURE);
      case XOR:
	printf("Instruction XOR\n");
	do_XOR(vm);
	break;
      case PUSH:
	printf("Memory : %p,  Memory value %d, SP : %p\n",
	       vm->memory,
	       *vm->memory,
	       vm->regs[SP]);
	do_PUSH(vm);
	break;
      case POP:
	printf("Pop\n");
	do_POP(vm);
	break;
      case MOV_L:
	do_MOV_L(vm);
	break;
      case MOV_H:
	do_MOV_H(vm);
	break;
      case ADD:
	do_ADD(vm);
	break;
      case CALL:
	do_CALL(vm);
	break;
      default:
	printf("Bad Instruction %d\n", *pInst);
	exit(EXIT_FAILURE);
	break;
      }
    pInst = get_pInstruction(vm);
    printf("PC:  %p\n", vm->regs[PC]);
    printf("pInst - vmcode = %d\n", pInst - vm->code);
  }
  printf("Done\n");
}

void check_reg(uint8_t reg)
{
  debug("Check register\n");
  if ((reg >= R_MAX)
      || (reg == PC)
      || (reg == SP))
    {
      printf("Bad Register\n");
      exit(EXIT_FAILURE);    
    }
}

void check_sp(vm_t* vm)
{
  debug("Check sp\n");
  printf("Memory : %p,  Memory value %d, SP : %p\n",
	 vm->memory,
	 *vm->memory,
	 vm->regs[SP]);
  if (( vm->regs[SP] - (uint64_t)vm->memory > VIRTMEM_SIZEMAX)
      || (vm->regs[SP] < (uint64_t)vm->memory))
    {
      printf("Bad SP pointer\n");
      exit(EXIT_FAILURE);
    }
  printf("SP OK.\n");
}

void check_pc(vm_t* vm)
{
  if (vm->code_len > CODE_SIZEMAX)
    exit(EXIT_FAILURE);
  if(( vm->regs[PC] - (uint64_t)vm->code > vm->code_len)
     || (vm->regs[PC] < (uint64_t)vm->code))
    {
      printf("Bad PC pointer\n");
      exit(EXIT_FAILURE);
    }
}

uint8_t* get_pInstruction(vm_t* vm)
{
  return (uint8_t*)vm->regs[PC]; 
}

void do_XOR(vm_t* vm)
{
  uint8_t* pInst = get_pInstruction(vm);
  check_reg(*(pInst+1));
  check_reg(*(pInst+2));
  
  vm->regs[*(pInst+1)] ^= vm->regs[*(pInst+2)];
  printf("Xored register %d with %d result %d\n",
	 *(pInst+1),
	 *(pInst+2),
	 vm->regs[*(pInst+1)]);
  vm->regs[PC]+= 3;
}

void do_PUSH(vm_t* vm)
{
  uint8_t* pInst = get_pInstruction(vm);
  check_reg(*(pInst+1));
  printf("Memory : %p,  Memory value %d, SP : %p\n",
	 vm->memory,
	 *vm->memory,
	 vm->regs[SP]);
  check_sp(vm);
  *((uint64_t*)(vm->regs[SP])) = (uint64_t)vm->regs[*(pInst+1)];
  vm->regs[SP]+= sizeof(uint64_t);
  printf("PC avant : %p\n",vm->regs[PC]);
  vm->regs[PC] += 2;
  printf("PC après : %p\n",vm->regs[PC]);
}

void do_POP(vm_t* vm)
{
  uint8_t* pInst = get_pInstruction(vm);
  check_reg(*(pInst+1));
  printf("Memory : %p,  Memory value %d, SP : %p\n",
	 vm->memory,
	 *vm->memory,
	 vm->regs[SP]);
  check_sp(vm);
  vm->regs[*(pInst+1)] = *((uint64_t*)(vm->regs[SP]));
  vm->regs[SP] -= sizeof(uint64_t);
  printf("PC avant : %p\n",vm->regs[PC]);
  vm->regs[PC] += 2;
  printf("PC après : %p\n",vm->regs[PC]);
}


void do_MOV_L(vm_t* vm)
{
  uint8_t* pInst = get_pInstruction(vm);
  check_reg(*(pInst+1));
  
  uint64_t value = 0;
  // big endian, because .
  
  value |= *(pInst+2);
  value |= *(pInst+3) << 0x8;
  value |= *(pInst+4) << 0x10;
  value |= *(pInst+5) << 0x18;

  printf("Value : %lld \n", value);
  value &= 0x00000000ffffffff;
  

  vm->regs[*(pInst+1)] &= 0xffffffff00000000;
  vm->regs[*(pInst+1)] |= value;

  vm->regs[PC] += 6;
}


void do_MOV_H(vm_t* vm)
{
  uint8_t* pInst = get_pInstruction(vm);
  check_reg(*(pInst+1));
  
  uint64_t value = 0;



  value |= (uint64_t)*(pInst+2) << 0x20;
  value |= (uint64_t)*(pInst+3) << 0x28;
  value |= (uint64_t)*(pInst+4) << 0x30;
  value |= (uint64_t)*(pInst+5) << 0x38;

  printf("Value : %llu \n", value);
  value &= 0xffffffff00000000;
  

  vm->regs[*(pInst+1)] &= 0x00000000ffffffff;
  vm->regs[*(pInst+1)] |= value;

  vm->regs[PC] += 6;
}



void do_ADD(vm_t* vm)
{
  uint8_t* pInst = get_pInstruction(vm);
  check_reg(*(pInst+1));
  check_reg(*(pInst+2));
  printf("R0 %d\n", vm->regs[*(pInst+1)]);
  vm->regs[*(pInst+1)] += vm->regs[*(pInst+2)];
  printf("R0 %d\n", vm->regs[*(pInst+1)]);
  vm->regs[PC] += 3;
}

void do_SUB(vm_t* vm)
{
  uint8_t* pInst = get_pInstruction(vm);
  check_reg(*(pInst+1));
  check_reg(*(pInst+2));
  printf("R0 %d\n", vm->regs[*(pInst+1)]);
  vm->regs[*(pInst+1)] -= vm->regs[*(pInst+2)];
  printf("R0 %d\n", vm->regs[*(pInst+1)]);
  vm->regs[PC] += 3;
}

void do_CALL(vm_t* vm)
{
  uint8_t* pInst = get_pInstruction(vm);
  char* file_path;
  FILE* f;
  uint64_t i;
  
  printf("Do Call\n");
  switch(*(pInst+1))
    {
      printf("Inst : %d", *(pInst+1));
    case F_BAD:
      printf("Bad instruction\n");
      exit(EXIT_FAILURE);
    case OPEN:
      check_sp(vm);
      file_path = (char*)vm->regs[SP];
      
      f = fopen(file_path, "r");
      if (f == NULL)
	{
	  printf("Failed to open file\n");
	  exit(EXIT_FAILURE);
	}
      else
	{
	  if (0)
	    {
	      // read
	      char buf[10];
	      fread((char*)buf, sizeof(char), 4, f);
	      buf[4] = '\0';
	      printf("Buf : %s\n", buf);
	    }
	 
	  vm->regs[R0] = (uint64_t)f;
	}
      vm->regs[PC] += 2;
      break;
    case READ:
      check_sp(vm);
      check_reg(*(pInst + 2));
      if ( 0x80 - ((uint64_t*)vm->regs[SP] - vm->memory )  <= 0x9)
	{
	  printf("Output buffer not big enough : %llu * 8 bytes\n",
		 (uint64_t*)vm->regs[SP] - vm->memory);
	  exit(EXIT_FAILURE);
	}
      for (i = 0x0; i < 0x9; i++)
	{
	  check_sp(vm);
	  printf("buffer : %llu  , i : %llu\n", *((uint64_t*)(vm->regs[SP]) + i ), i);
	  if ( (uint64_t)*((uint64_t*)(vm->regs[SP]) + i ) != 0)
	    {
	      printf("Output buffer must be zeroed\n");
	      exit(EXIT_FAILURE);
	    }
	}
      
      fread((uint64_t*)vm->regs[SP],
	    sizeof(char),
	    0x20,
	    (FILE*)vm->regs[*(pInst+2)]);

      if(1)
	{
	  printf("Buf : %s\n", vm->regs[SP]);     
	  for (i = 0x0; i < 0x9; i++)
	    {
	      check_sp(vm);
	      printf("buffer out : %llu  , i : %llu\n", *((uint64_t*)(vm->regs[SP]) + i ), i); 
	    }
	}
      vm->regs[PC] += 3;
      break;
    case WRITE:
      if( 0x80 - ((uint64_t*)vm->regs[SP] - vm->memory)<= 0x9)
	{
	  printf("Will read out of bounds... \n");
	  exit(EXIT_FAILURE);
	}
	  
      check_sp(vm);
      check_reg(*(pInst+2));
      
      if (vm->regs[*(pInst+2)] > 0x4)
	{
	  printf("Wrong fd : %llu\n", vm->regs[*(pInst+2)]);
	  exit(EXIT_FAILURE);
	}

      // Write onto fd in register

      if(1)
	{
	  printf("Buf : %s\n", vm->regs[SP]);     
	  for (i = 0x0; i < 0x9; i++)
	    {
	      check_sp(vm);
	      printf("buffer out : %llu  , i : %llu\n", *((uint64_t*)(vm->regs[SP]) + i ), i); 
	    }
	}
      
      write(vm->regs[*(pInst+2)], (uint64_t*)(vm->regs[SP]), 0x20);


      vm->regs[PC]+= 3;
      break;
	  
    default:
      printf("Nop");
      break;
    }  
}
