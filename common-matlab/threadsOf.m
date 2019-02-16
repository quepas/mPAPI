function spid = threadsOf(pid)
    [~, spid] = system(['ps -T -p ', num2str(pid), ' -o spid']);
    spid = str2double(regexp(spid, '\d*', 'Match'));
    spid = spid(spid ~= pid);
end
