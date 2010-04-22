program select_and_cal
  use permanent
  use mykind
  use timer
  implicit none
  integer(matrix_type),dimension(:,:),allocatable::m,m1
  integer,dimension(:),allocatable::rows,cols
  character(len=100)::load_path,selection_path,buffer
  integer :: num_selection,skip
  integer(matrix_type)::result
  real(8)::time
  integer :: i

  if ( command_argument_count()/=4 ) then
     write (0,*) "usage: select_and_cal num skip matrix_file selection_file"
     return
  end if
  call get_command_argument(1,buffer)
  read (buffer,*) num_selection
  call get_command_argument(2,buffer)
  read (buffer,*) skip
  call get_command_argument(3,load_path)
  call get_command_argument(4,selection_path)
  open(unit=11,file=selection_path)
  call load(m,load_path)
  allocate(m1(size(m,1),size(m,2)),rows(num_selection),cols(num_selection))
  
  do i=1,2*skip
	  read (11,*)
  end do
  call generate_traval_order
  do 
     m1=m
     read (11,*,end=10) (rows(i),i=1,num_selection)
     read (11,*,end=10) (cols(i),i=1,num_selection)
     do i=1,num_selection
        m1(rows(i)+1,cols(i)+1)=0
     end do
     call tic
     result=H(m1)
     call toc(time)
     write (*,*) size(m1,1),result,time
  end do

10 close(11)
  deallocate(m,m1,rows,cols)
end program select_and_cal
