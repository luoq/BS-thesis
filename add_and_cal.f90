program select_and_cal
  use permanent
  use mykind
  use timer
  implicit none
  integer(matrix_type),dimension(:,:),allocatable::m
  integer,dimension(:),allocatable::cols
  character(len=100)::matrix_path,add_path,buffer
  integer :: num_add,line
  integer(matrix_type)::result
  real(8)::time,time1
  integer :: i

  if ( command_argument_count()/=4 ) then
     write (0,*) "usage: select_and_cal matrix_file selection_file line how_many_add"
     return
  end if
  call get_command_argument(1,matrix_path)
  call get_command_argument(2,add_path)
  call get_command_argument(3,buffer)
  read (buffer,*) line
  call get_command_argument(4,buffer)
  read (buffer,*) num_add

  open(unit=11,file=add_path)
  call load(m,matrix_path)
  allocate(cols(num_add))
  
  do i=1,line-1
	  read (11,*)
  end do
  read (11,*,end=10) (cols(i),i=1,num_add)
  call printMatrix(m)
  do i=1,num_add
	  m(i,cols(i)+1)=1
  end do
  write (*,*) 
  call printMatrix(m)

  call generate_traval_order
  call tic
  result=H(m)
  call toc(time)
  if ( time<1e-1 ) then
     do
        call tic
        result=H(m)
        call toc(time1)
        if ( time1>=(1+0.0)*time ) then
           exit
        end if
        time=time1
     end do
  end if
  write (*,*) size(m,1),result,time

10 close(11)
  deallocate(m,cols)
end program select_and_cal
