<h1>minishell</h1>
<p>Code a simplified shell in C</p>
<p>Group project for school 42 Paris, done with @sequoiageant&nbsp;</p>
<p>Original git repo: https://github.com/Sequoiageant/minishell&nbsp;</p>
<h2>Assignement:</h2>
<p>Your shell should:&nbsp;</p>
<ul style="list-style-type: circle;">
    <li>Show a prompt when waiting for a new command&nbsp;</li>
    <li>Search and launch the right executable (based on the PATH variable or by using relative or absolute path) like in bash&nbsp;</li>
    <li>It must implement the builtins like in bash:&nbsp;<ul>
            <li style="margin-left: 20px;">echo with option &rsquo;-n&rsquo;&nbsp;</li>
            <li style="margin-left: 20px;">cd with only a relative or absolute path&nbsp;</li>
            <li style="margin-left: 20px;">pwd without any options&nbsp;</li>
            <li style="margin-left: 20px;">export without any options ◦ unset without any options&nbsp;</li>
            <li style="margin-left: 20px;">env without any options and any arguments&nbsp;</li>
            <li style="margin-left: 20px;">exit without any options</li>
        </ul>
    </li>
    <li>; in the command should separate commands like in bash&nbsp;</li>
    <li>&rsquo; and &quot; should work like in bash except for multiline commands&nbsp;</li>
    <li>Redirections &lt; &gt; &ldquo;&gt;&gt;&rdquo; should work like in bash except for file descriptor aggregation&nbsp;</li>
    <li>Pipes | should work like in bash&nbsp;</li>
    <li>Environment variables ($ followed by characters) should work like in bash&nbsp;</li>
    <li>$? should work like in bash&nbsp;</li>
    <li>ctrl-C, ctrl-D and ctrl-\ should have the same result as in bash&nbsp;</li>
</ul>
<h2>Key functions:</h2>
<ul style="list-style-type: circle;">
    <li>Pipe&nbsp;</li>
    <li>Fork&nbsp;</li>
    <li>Wait / Waitpid&nbsp;</li>
    <li>Signal&nbsp;</li>
    <li>Dup</li>
</ul>
<h2>Ressources</h2>
<p>On how shell works: <a href="https://indradhanush.github.io/blog/writing-a-unix-shell-part-1/">https://indradhanush.github.io/blog/writing-a-unix-shell-part-1/</a></p>
<p>Man Bash: <a href="https://docs.google.com/document/d/1Md5AhIjY-1H6xenDQ8D-qWu5CGWgze6Q3J14B6_FQu0/edit?usp=sharing">https://docs.google.com/document/d/1Md5AhIjY-1H6xenDQ8D-qWu5CGWgze6Q3J14B6_FQu0/edit?usp=sharing</a></p>
<p>POSIX 2017: <a href="https://pubs.opengroup.org/onlinepubs/9699919799/nframe.html">https://pubs.opengroup.org/onlinepubs/9699919799/nframe.html</a></p>
<p>Introduction to pipes: <a href="https://tldp.org/LDP/lpg/node10.html#SECTION00721000000000000000">https://tldp.org/LDP/lpg/node10.html#SECTION00721000000000000000</a></p>
<p>Multiple commands pipelines: <a href="http://web.cse.ohio-state.edu/~mamrak.1/CIS762/pipes_lab_notes.html">http://web.cse.ohio-state.edu/~mamrak.1/CIS762/pipes_lab_notes.html</a></p>
<p><br></p>
<h2>Tests:&nbsp;</h2>
<p>https://docs.google.com/spreadsheets/d/1pD3UEKayYV_LiHoYz8N6aOimqS9gxfg3TlZgQ1CFS3A/edit#gid=1627853444</p>
