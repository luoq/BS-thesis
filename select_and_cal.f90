program select_and_cal
  use permanent
  use mykind
  use timer
  implicit none
  integer(matrix_type),dimension(:,:),allocatable::m
  integer,dimension(:),allocatable::rows,cols
  character(len=100)::load_path,selection_path,buffer
  integer :: num_selection,line
  integer(matrix_type)::result
  real(8)::time
  integer :: i

  if ( command_argument_count()/=4 ) then
     write (0,*) "usage: select_and_cal num line matrix_file selection_file"
     return
  end if
  call get_command_argument(1,buffer)
  read (buffer,*) num_selection
  call get_command_argument(2,buffer)
  read (buffer,*) line
  call get_command_argument(3,load_path)
  call get_command_argument(4,selection_path)
  open(unit=11,file=selection_path)
  call load(m,load_path)
  allocate(rows(num_selection),cols(num_selection))
  
  do i=1,2*(line-1)
	  read (11,*)
  end do
  read (11,*,end=10) (rows(i),i=1,num_selection)
  read (11,*,end=10) (cols(i),i=1,num_selection)
  do i=1,num_selection
	  m(rows(i)+1,cols(i)+1)=0
  end do

  call generate_traval_order
  call tic
  result=H1(m)
  call toc(time)
  write (*,*) size(m,1),result,time

10 close(11)
  deallocate(m,rows,cols)
end program select_and_cal
