program Per
  use permanent
  use timer
  use mykind
  implicit none
  integer(matrix_type),dimension(:,:),allocatable :: m
  integer(matrix_type)::result
  character(len=100)::path
  real(8) :: time
#ifdef debug
#endif
  call get_command_argument(1,path) 
  call load(m,path)
  m(13,1)=0
  !write (*,*) IDEM(m)
  !write (*,*) RNW(m)
  
  call tic
  call generate_traval_order
  result=H(m)
  call toc(time)
  write (*,*) size(m,1),result,time

  deallocate(m)
end program Per
