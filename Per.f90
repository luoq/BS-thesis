program Per
  use permanent
  use timer
  implicit none
  integer(matrix_type),dimension(:,:),allocatable :: m
  character(len=100)::path
  real(8) :: time
#ifdef debug
#endif
  call get_command_argument(1,path)
  call load(m,path)
  !write (*,*) IDEM(m)
  !write (*,*) RNW(m)
  call tic
  write (*,*) H(m)
  call toc(time)
  write (*,*) 'Time elapsed:',time,'ms'
  deallocate(m)
end program Per
