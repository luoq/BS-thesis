program test
  call test_timer
end program test
subroutine test_gen_subsets
  use gen_subsets
  implicit none
  integer::next,add_or_remove,k
  logical::end
  call initialize_sub(8)
  do 
     call next_sub(next,add_or_remove,k,end)
     write (*,*) 'next:',next,'add?:',add_or_remove,'cardinality',k
     if(end) exit
  end do
end subroutine test_gen_subsets
subroutine test_timer
  use timer
  implicit none
  real time
  real x,result
  call tic
  do x=1,100
     result=sin(x)
  end do
  call toc(time)
  write (*,*) 'Elapsed time:',time
end subroutine test_timer


