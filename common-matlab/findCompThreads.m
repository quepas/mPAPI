function compThreads = findCompThreads()
    repeats = 100;
    % Force use of many-threads
    % Repeat a few times to use all threads
    for k = 1:repeats
        A = rand(1, 1000000);
        B = rand(1, 1000000);
        C = A + B;
    end

    % Assumption: only one MATLAB runs at a time!
    pid = pidOf('MATLAB');
    % Get child threads/processes of MATLAB
    spid = threadsOf(pid);

    compThreads = [];

    for k = 1:repeats
        % Find threads that perform at least some work
        for thread = spid
            ev = mPAPI_register({'PAPI_TOT_CYC'}, thread);
            mPAPI_tic(ev);
            C = A + B;
            num_ops = mPAPI_toc(ev);

            if num_ops > 0
                compThreads = [compThreads, thread];
            end

        end

    end

    compThreads = unique(compThreads);

end
