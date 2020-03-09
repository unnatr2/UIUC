.data					# section declaration

        # Useful offset constants for accessing members of a 
        # struct mp1_blink_struct structure
        LOCATION   = 0    
        ON_CHAR    = 2
        OFF_CHAR   = 3 
        ON_LENGTH  = 4
        OFF_LENGTH = 6
        COUNTDOWN  = 8
        STATUS     = 10
        NEXT       = 12

        STRUCT_SIZE = 16

# Maximum possible LOCATION of character on screen (80*25-1)
MAX_LOCATION:
        .word 2000                      

# Pointer to head of list (initialized to NULL)
mp1_list_head:
        .long   0

.text					# section declaration

# Export the function symbol names

.global mp1_rtc_tasklet
.global mp1_ioctl





# JUMP TABLE FOR IOCTL

jmp_table:
        .long mp1_ioctl_add, mp1_ioctl_remove, mp1_ioctl_find, mp1_ioctl_sync





# void mp1_poke(void);
#
# Interface: Register-based arguments (not C-style)
#    Inputs: %cl  - The byte you wish to write
#            %eax - Offset from the start of video memory that you wish
#                   to write to
#   Outputs: Text-mode video screen is written to at location %eax with
#            the byte in %cl
# Registers: Clobbers EDX

mp1_poke:
        
	movl    vmem_base_addr(,1),%edx
	movb    %cl,(%edx,%eax,1)
	ret





# MP1 RTC TASKLET
# 
# Prototype: void mp1_rtc_tasklet (unsigned long);
# Purpose: Called when RTC interrupt is generated.
# Implementation: Traverse mp1_list_head list and update COUNTDOWN and
#                 STATUS of each mp1_blink_struct. If COUNTDOWN reaches 0,
#                 change character and update COUNTDOWN to other char.
# Registers: ESI - Holds pointer value of mp1_blink_struct element in list.
#            CL  - Holds ASCII value of character to be printed on screen.
#            EAX - Holds LOCATION of the character from the start of video memory.

mp1_rtc_tasklet:
        # Save old EBP and move EBP to ESP
        pushl %ebp
        movl %esp, %ebp

        # Save callee saved registers
        pushl %ebx
        pushl %edi
        pushl %esi

        # Set list head
        movl mp1_list_head, %esi

tasklet_loop_head:
        # Check if reached list end
        cmpl $0, %esi
        je mp1_rtc_tasklet_done

        # Decrement COUNTDOWN
        decw COUNTDOWN(%esi)

        # Check if COUNTDOWN ended
        cmpw $0, COUNTDOWN(%esi)
        jg tasklet_loop_next

        # Check STATUS
        cmpw $0, STATUS(%esi)
        je mp1_rtc_tasklet_off
        jne mp1_rtc_tasklet_on

mp1_rtc_tasklet_on:
        # Call mp1_poke to print OFF_CHAR
        movb OFF_CHAR(%esi), %cl
        movzwl LOCATION(%esi), %eax
        addl %eax, %eax                         # LOCATION*2 (since stored in low byte)
        pushl %edx
        call mp1_poke
        popl %edx

        # Reset COUNTDOWN
        movw OFF_LENGTH(%esi), %dx
        movw %dx, COUNTDOWN(%esi)

        # Update STATUS
        movw $0, STATUS(%esi)
        jmp tasklet_loop_next

mp1_rtc_tasklet_off:
        # Call mp1_poke to print ON_CHAR
        movb ON_CHAR(%esi), %cl
        movzwl LOCATION(%esi), %eax
        addl %eax, %eax                         # LOCATION*2 (since stored in low byte)
        pushl %edx
        call mp1_poke
        popl %edx

        # Reset COUNTDOWN
        movw ON_LENGTH(%esi), %dx
        movw %dx, COUNTDOWN(%esi)

        # Update STATUS
        movw $1, STATUS(%esi)
        jmp tasklet_loop_next

tasklet_loop_next:
        # Move to next element in list
        movl NEXT(%esi), %esi
        jmp tasklet_loop_head

mp1_rtc_tasklet_done:
        # Restore registers
        popl %esi
        popl %edi
        popl %ebx

        # Return
        leave
        ret





# MP1 IOCTL
# 
# Prototype: int mp1_ioctl (unsigned long arg, unsigned long cmd);
# Purpose: Serves as dispatcher function to jump to various I/O Controls.
# Implementation: Use parameter cmd to determine which I/O Control to jump to.
#                 Uses a jump table to jump to specified functions. Returns -1 if
#                 cmd parameter is invalid.
# Registers: EAX - Holds cmd parameter.

mp1_ioctl:
        # Move cmd to EAX
        movl 8(%esp), %eax                      # cmd = ESP+8

        # Check if cmd < 0
        cmpl $0, %eax
        jl invalid_cmd

        # Check if cmd > 3
        cmpl $3, %eax
        jg invalid_cmd

        # Dispatch to specified ioctl
        jmp *jmp_table(,%eax,4)
        jmp mp1_ioctl_done
        
invalid_cmd:
        # Return -1
        movl $-1, %eax

mp1_ioctl_done:
        # Return
	ret




# MP1 IOCTL ADD
# 
# Prototype: int mp1_ioctl_add (unsigned long arg);
# Purpose: Allocates and prints character with properties as passed.
# Implementation: Dynamically allocate memory for mp1_blink_struct. Set COUNTDOWN
#                 and STATUS fields. Insert element into mp1_list_head list. Finally,
#                 print specified character on screen. Return 0 if successful, -1 otherwise.
# Registers: EAX - Holds arg parameter, malloc pointer, LOCATION of the character
#                  from the start of video memory and memcopy remainder bits.
#            BX  - Holds MAX LOCATION possible (80*25-1).
#            CL  - Holds ASCII value of character to be printed on screen.          

mp1_ioctl_add:
        # Save old EBP and move EBP to ESP
        pushl %ebp
        movl %esp, %ebp

        # Save callee saved registers
        pushl %ebx
        pushl %edi
        pushl %esi

        # Check if arg is valid
        cmpl $0, 8(%ebp)                        # arg = EBP+8
        je mp1_ioctl_add_arg_error

        # Check if LOCATION is valid
        movl 8(%ebp), %eax
        cmpw $0, LOCATION(%eax)
        jl mp1_ioctl_add_location_error
        movw MAX_LOCATION, %bx
        cmpw %bx, LOCATION(%eax)
        jg mp1_ioctl_add_location_error

        # Call malloc
        pushl $STRUCT_SIZE                      # size = 16
        call mp1_malloc
        addl $4, %esp                           # teardown stack

        # Check if memory allocation worked
        cmpl $0, %eax
        je mp1_ioctl_add_malloc_error
        movl %eax, %ebx                         # Save EAX

        # Copy blink struct to allocated memory
        pushl $STRUCT_SIZE                      # n = 16
        pushl 8(%ebp)                           # from = arg
        pushl %eax                              # to = malloc pointer
        call mp1_copy_from_user
        addl $12, %esp                          # teardown stack

        # Check if memory was copied successfully
        cmpl $0, %eax
        jne mp1_ioctl_add_memcopy_error

        # Set COUNTDOWN and STATUS
        movl %ebx, %eax                         # Restore EAX
        movl ON_LENGTH(%eax), %edi
        movl %edi, COUNTDOWN(%eax)
        movl $1, STATUS(%eax)

        # Set struct to head of linked list
        movl mp1_list_head, %edi
        movl %edi, NEXT(%eax)
        movl %eax, %edi
        movl %edi, mp1_list_head

        # Call mp1_poke
        movb ON_CHAR(%eax), %cl
        movl %eax, %ebx                         # Save EAX
        movzwl LOCATION(%ebx), %eax
        addl %eax, %eax                         # LOCATION*2 (since stored in low byte)
        pushl %edx
        call mp1_poke
        popl %edx
        movl %ebx, %eax                         # Restore EAX

        # Add successful
        jmp mp1_ioctl_add_successful


mp1_ioctl_add_successful:
        # Return 0
        movl $0, %eax
        jmp mp1_ioctl_add_return


mp1_ioctl_add_memcopy_error:
        # Free allocated memory
        pushl %eax
        call mp1_free
        addl $4, %esp                           # teardown stack

mp1_ioctl_add_arg_error:

mp1_ioctl_add_malloc_error:

mp1_ioctl_add_location_error:
        # Return -1
        movl $-1, %eax
        jmp mp1_ioctl_add_return

mp1_ioctl_add_return:
        # Restore registers
        popl %esi
        popl %edi
        popl %ebx

        # Return
        leave
        ret




# MP1 IOCTL REMOVE
# 
# Prototype: int mp1_ioctl_remove (unsigned long arg);
# Purpose: Removes element from mp1_list_head list.
# Implementation: Traverse mp1_list_head list and find element with specified LOCATION.
#                 Remove element from list and free allocated memory. Return 0 if successful,
#                 -1 otherwise.
# Registers: EAX - Holds linked list element.
#            BX  - Holds LOCATION from arg parameter.
#            ESI - Holds mp1_blink_struct pointer to element in list.

mp1_ioctl_remove:
        # Save old EBP and move EBP to ESP
        pushl %ebp
        movl %esp, %ebp

        # Save callee saved registers
        pushl %ebx
        pushl %edi
        pushl %esi

        # Save LOCATION
        movw 8(%ebp), %bx                       # arg = EBP+8

        # Search list for element
        pushw %bx
        call linked_list_search
        addl $2, %esp                           # teardown stack
        cmpl $0, %eax
        je remove_failed

        # Check if element at head
        movl mp1_list_head, %esi 
        cmpw %bx, LOCATION(%esi)
        je remove_head
        jne remove_middle_or_end

remove_head:
        # Remove element from list
        movl NEXT(%esi), %ebx
        movl %ebx, mp1_list_head
        
        # Call mp1_free
        pushl %esi
        call mp1_free
        addl $4, %esp                           # teardown stack
        jmp remove_successful

remove_middle_or_end:
        # Check if element is next
        cmpl NEXT(%esi), %eax
        jne remove_loop_end

        # Remove element from list
        movl NEXT(%eax), %ebx
        movl %ebx, NEXT(%esi)

        # Call mp1_free
        pushl %eax
        call mp1_free
        addl $4, %esp                           # teardown stack
        jmp remove_successful

remove_loop_end:
        # Move to next element in list
        movl NEXT(%esi), %esi
        jmp remove_middle_or_end

remove_successful:
        # Return 0
        movl $0, %eax
        jmp mp1_ioctl_remove_return

remove_failed:
        # Return -1
        movl $-1, %eax

mp1_ioctl_remove_return:
        # Restore registers
        popl %esi
        popl %edi
        popl %ebx

        # Return
        leave
        ret





# MP1 IOCTL FIND
# 
# Prototype: int mp1_ioctl_find (unsigned long arg);
# Purpose: Finds and saves mp1_blink_struct from memory to user-level structure.
# Implementation: Traverse mp1_list_head list and find element that matches 'arg' LOCATION.
#                 Copy this element from memory into user-level structure. Return 0 is successful,
#                 -1 otherwise.
# Registers: EAX - Holds linked list element and memcopy remainder bits.
#            BX  - Holds LOCATION from arg parameter.
        
mp1_ioctl_find:
        # Save old EBP and move EBP to ESP
        pushl %ebp
        movl %esp, %ebp

        # Save callee saved registers
        pushl %ebx
        pushl %edi
        pushl %esi

        # Check if arg is valid
        cmpl $0, 8(%ebp)                        # arg = EBP+8
        je mp1_ioctl_find_arg_error

        # Save LOCATION
        movw 8(%ebp), %bx                       # arg = EBP+8

        # Search list for element
        pushw %bx
        call linked_list_search
        addl $2, %esp                           # teardown stack           
        cmpl $0, %eax
        je mp1_ioctl_find_failed

        # Call memcopy
        pushl $STRUCT_SIZE                      # n = 16
        pushl %eax                              # from = list element
        pushl 8(%ebp)                           # to = arg
        call mp1_copy_to_user
        addl $12, %esp                          # teardown stack
        cmpl $0, %eax
        je mp1_ioctl_find_successful
        jne mp1_ioctl_find_failed

mp1_ioctl_find_failed:
mp1_ioctl_find_arg_error:
find_loop_failed:
        # Return -1
        movl $-1, %eax
        jmp mp1_ioctl_find_return

mp1_ioctl_find_successful:
        # Return 0
        movl $0, %eax
        jmp mp1_ioctl_find_return

mp1_ioctl_find_return:
        # Restore registers
        popl %esi
        popl %edi
        popl %ebx

        # Return
        leave
        ret




# MP1 IOCTL SYNC
# 
# Prototype: int mp1_ioctl_sync (unsigned long arg);
# Purpose: Synchronizes two existing locations on the screen.
# Implementation: Split arg parameter to find locations of 1st and 2nd characters. Traverse
#                 mp1_list_head list to find these elements. Finally, update the ON_LENGTH,
#                 OFF_LENGTH, COUNTDOWN and STATUS fields of 2nd as per 1st. Update the 2nd
#                 LOCATION to display correct character. Return 0 if successful, -1 otherwise.
# Registers: EAX - Holds linked list element and LOCATION of character.
#            BX  - Holds LOCATION of 1st element.
#            CX  - Holds LOCATION of 2nd element.
#            ESI - Holds 1st element pointer.
#            EDI - Holds 2nd element pointer.
#            CL  - Holds ASCII value of character to be printed on screen.          

mp1_ioctl_sync:
        # Save old EBP and move EBP to ESP
        pushl %ebp
        movl %esp, %ebp

        # Save callee saved registers
        pushl %ebx
        pushl %edi
        pushl %esi

        # Save and split arg parameter
        movl 8(%ebp), %ebx                      # arg = EBP+8
        movw %bx, %cx                           # 2nd integer in CX
        shrl $16, %ebx                          # 1st integer in BX

        # Find 1st integer in list
        pushw %bx                       
        call linked_list_search
        addl $2, %esp                           # teardown stack
        cmpl $0, %eax
        je mp1_ioctl_sync_failed

        # Save element
        movl %eax, %esi

        # Find 2nd integer in list
        pushw %cx
        call linked_list_search
        addl $2, %esp                           # teardown stack
        cmpl $0, %eax
        je mp1_ioctl_sync_failed

        # Save element
        movl %eax, %edi

        # Clear EDX
        xorl %edx, %edx

        # Change ON_LENGTH, OFF_LENGTH, COUNTDOWN and STATUS fields
        movw ON_LENGTH(%esi), %dx
        movw %dx, ON_LENGTH(%edi)
        movw OFF_LENGTH(%esi), %dx
        movw %dx, OFF_LENGTH(%edi)
        movw COUNTDOWN(%esi), %dx
        movw %dx, COUNTDOWN(%edi)
        movw STATUS(%esi), %dx
        movw %dx, STATUS(%edi)

        # Check status
        cmpw $0, STATUS(%edi)
        je mp1_ioctl_sync_off
        jne mp1_ioctl_sync_on

mp1_ioctl_sync_on:
        # Call mp1_poke to turn on
        movb ON_CHAR(%edi), %cl
        movzwl LOCATION(%edi), %eax
        addl %eax, %eax                         # LOCATION*2 (since stored in low byte)
        pushl %edx
        call mp1_poke
        popl %edx
        jmp mp1_ioctl_sync_successful

mp1_ioctl_sync_off:
        # Call mp1_poke to turn off     
        movb OFF_CHAR(%esi), %cl
        movzwl LOCATION(%esi), %eax
        addl %eax, %eax                         # LOCATION*2 (since stored in low byte)
        pushl %edx
        call mp1_poke
        popl %edx
        jmp mp1_ioctl_sync_successful


mp1_ioctl_sync_failed:
        # Return -1
        movl $-1, %eax
        jmp mp1_ioctl_sync_return

mp1_ioctl_sync_successful:
        # Return 0
        movl $0, %eax
        jmp mp1_ioctl_sync_return

mp1_ioctl_sync_return:
        # Restore registers
        popl %esi
        popl %edi
        popl %ebx

        # Return
        leave
        ret





# LINKED LIST SEARCH (HELPER)
# 
# Prototype: void* linked_list_search (LOCATION);
# Purpose: Helper to find element with given LOCATION in mp1_list_head list.
# Implementation: Traverse through mp1_list_head list and find element with specified LOCATION.
#                 Return pointer to list element if found, 0 otherwise.  
# Registers: EAX - Holds linked list element.
#            BX  - Holds LOCATION of element.         

linked_list_search:
        # Save old EBP and move EBP to ESP
        pushl %ebp
        movl %esp, %ebp

        # Save callee saved registers
        pushl %ebx
        pushl %edi
        pushl %esi

        # Get LOCATION
        movw 8(%ebp), %bx                      # LOCATION = EBP+8         

        # Get list head
        movl mp1_list_head, %eax

 search_loop_head:
        # Check if reached list end
        cmpl $0, %eax
        je search_failed

        # Check if LOCATION matches
        cmpw %bx, LOCATION(%eax)
        je search_found

        # Move to next element in list
        movl NEXT(%eax), %eax
        jmp search_loop_head

search_failed:
        # Return value -> 0
        movl $0, %eax       

search_found:
        # Restore registers
        popl %esi
        popl %edi
        popl %ebx

        # Return
        leave
        ret

.end
