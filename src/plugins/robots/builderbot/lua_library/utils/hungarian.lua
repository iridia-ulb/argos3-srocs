robot.logger:register_module("utils.hungarian")

-- get the size of a table, no matter square or not
local function get_nm_from_mat(Mat)
   --[[
   find the size of a Mat
   i.e.
        M=4
       * * * *
   N=3 * * * *
       * * * *

   return 3,4
   --]]

   --Asserts
      --checked in for
      --presume Mat is like {{a,a},{a,a,a},{a,a}}

   local maxM = 0
   local i = 0
   for ii,v in ipairs(Mat) do
      i = ii
      if type(v) ~= "table" then return -1,-1 else
         for jj,t in ipairs(v) do j=jj end
         if j>maxM then maxM = j end
      end
   end
   return i,maxM
end

----------------------------------------------------------------
-- Hungarian starts
   -- for the algorithm, please refer to: 
   -- https://www.topcoder.com/community/data-science/data-science-tutorials/assignment-problem-and-hungarian-algorithm/

------------------------------------------------------------------------------------------
local function update_labels(self)
   local N = self.N
   local slack = self.slack
   local slackx = self.slackx

   local delta = slack[1] + 99999999  -- for max set as INF

   -- find the min delta among slack
   for y = 1,N do
      if self.T[y] ~= true and slack[y] < delta then  
         delta = slack[y]
      end
   end

   -- update delta change
   for x = 1,N do
      if self.S[x] == true then
         self.lx[x] = self.lx[x] - delta     --max
      end
   end
   for y = 1,N do
      if self.T[y] == true then
         self.ly[x] = self.ly[x] + delta     --max
      end
   end

   -- update slack
   for y = 1,N do
      if self.T[y] ~= true then slack[y] = slack[y] - delta  end --max
   end
end

local function add_to_tree(self, x, its_parent)
   self.S[x] = true
   self.parent_table[x] = its_parent
   
   -- update slack of this new x in S
   for y = 1,self.N do
      if (self.lx[x] + self.ly[y] - self.cost_matrix[x][y] < self.slack[y]) then        --max
         self.slack[y] = self.lx[x] + self.ly[y] - self.cost_matrix[x][y]      -- max
         self.slackx[y] = x
      end
   end
end

---The Augment-------------------------------------------------------------------------------
local function aug_function(self)
   --[[
      for someone not be matched in X:
         1. try to find all his augmenting tree, 
            if a path is found, goto the end, change the match and recur aug() 
         2. if all the augmenting tree is set and no path found, update label
         3. keep finding, should find some new edges, if not, this is the answer
   --]]

   -- OK already?
   if (self.maxMatch == self.N) then return 0 end
   local N = self.N
      -- write self.N everytime could be annoying, use N directly

   ---------------------------------------------------------------------
   -- Start to Build tree ----------------------------------------------
      -- using S,T,
      -- and a queue ?? 
         --the use of the queue is xxxxx
      -- a slack for find the min delta quickly

   ----Find a single x and Init everything--------------

   -- Init everything
   self.S = {}
   self.T = {}
   self.parent_table = {}
   local S = self.S     -- frequently used, so no need of self.xx every time
   local T = self.T
   local parent_table = self.parent_table
   local queue = robot.utils.queue.create()
   local root 

   -- find a single x
   for x = 1,N do
      if self.match_of_X[x] == nil then
         queue:push(x)
         root = x
         parent_table[x] = -2
         S[x] = true
         break
      end
   end
      --must find a single x, or function should have returned checking maxMatch
   
   -- init slack
      -- slack is used for store the mini delta for each y
      -- slackx is used for store to which x this mini delta is achieved
   self.slack = {} self.slackx = {}
   local slack = self.slack 
   local slackx = self.slackx

   for y = 1,N do 
      slack[y] = self.lx[root] + self.ly[root] - self.cost_matrix[root][y] end    -- max
   for y = 1,N do slackx[y] = root end

   -----Start to find------------------------------------

   local edgex = nil    -- used for record the edge if a good path is found
   local edgey = nil
   local flag = 0 --flag = 1 mean found a good path
   while true do
      -- search every x in queue
      while queue:empty() == false do
         x = queue:pop()
         -- for this new x, search all its edges
         flag = 0 --flag = 1 mean found a good path
         for y = 1,N do
            -- search y, find a edge of equality between this new x and a new y (not in T)
            if self.cost_matrix[x][y] == self.lx[x] + self.ly[y] and T[y] ~= true then
               -- don't write T[y] == false, because T[y] == nil initially
               -- check is this y assigned? if not means a good path found
               if self.match_of_Y[y] == nil then 
                  edgex = x; edgey = y
                  flag = 1; break 
               end  
                  -- a good path found, jump out of for y(search for y) 

               -- y is assigned, add the x of this y to tree
               T[y] = true
               queue:push(self.match_of_Y[y])
               --S[x] = true   -- this is done in add_to_tree
               add_to_tree(self, self.match_of_Y[y],x)
            end
         end   -- end of for y

         if flag == 1 then break end
            -- a good path found jump out of while queue)
            -- else next x in queue
      end   -- end of queue searching (while queue:isEmpty)

      if flag == 1 then break end
         -- a good path found jump out of this x searching
         -- else means we have searched every x in S
         --             tree is built
         --          need to change label next
      
      update_labels(self)
   
      queue = robot.utils.queue.create()
      flag = 0
      for y = 1,N do
         if T[y] ~= true and slack[y] == 0 then
            -- means a new equal edge is found, a new Y
            if self.match_of_Y[y] == nil then
               -- this Y is single, record and break
               x = slackx[y]
               flag = 1
               edgex = x; edgey = y
               break
            else
               -- this Y is not single, add to tree
               T[y] = true
               if S[self.match_of_Y[y]] == nil then
                  queue:push(self.match_of_Y[y])
                  add_to_tree(self, self.match_of_Y[y],slackx[y])
               end
            end
         end
      end

      if flag == 1 then break end
      -- if a good path found, jump out, otherwise keep searching x
   end   -- end of x searching (while true)

   if flag == 1 then  -- a good path
      self.maxMatch = self.maxMatch + 1
      local px = edgex
      local py = edgey
      local temp
      while px ~= -2 do
         temp = self.match_of_X[px]
         self.match_of_Y[py] = px
         self.match_of_X[px] = py
         py = temp
         px = parent_table[px]
      end
      self:aug()
   end

   -- if it comes here, that means all the possibility is tried and no other edges can be added,
   -- then it is the end, match table is what we got
end   --end of function aug

package.loaded['utils.hungarian'] = {
   create = function(cost_matrix_in, is_it_max_problem)
      local instance = {
         -- a Hungarian should have these data
         cost_matrix = {},
         N = 0,
         -- M = 0
            -- no M currently, consider only square
         maxMatch = 0,
         match_of_X = nil,
         match_of_Y = nil,
   
         aug = aug_function,
      }
   
      -- Set cost_matrix and size N
      local n,m = get_nm_from_mat(cost_matrix_in)
      
      -- check and get N
      if n == -1 or m == -1 then
         robot.logger:log_err("invalid cost matrix")
         return nil
      end
      if n ~= m then
         robot.logger:log_err("cost matrix is not square")
         -- to be filled
         return nil
            -- temporarily
      end
      instance.N = n

      for i = 1, n do
         instance.cost_matrix[i] = {}
         for j = 1, m do
            instance.cost_matrix[i][j] = cost_matrix_in[i][j] or 0
         end
      end
   
      ---------------- min or max problem ----------------
      if is_it_max_problem == false then
         for i = 1,n do
            for j = 1,n do
               instance.cost_matrix[i][j] = -instance.cost_matrix[i][j] 
            end
         end
      end
      ----------------------------------------------------
   
      -- Set labels and maxMatch
      instance.maxMatch = 0
      instance.match_of_X = {}
      instance.match_of_Y = {}
   
      --init lx,ly, which are the value labels of X and Y
      instance.lx = {}; instance.ly = {}
      -- set label of Y with all 0
      for i = 1,n do instance.ly[i] = 0 end
      -- set label of X with the max of his cost edges -- for max problem
      for i = 1,n do instance.lx[i] = -math.huge end
      for i = 1,n do 
         for j = 1,n do
            if instance.lx[i] < instance.cost_matrix[i][j] then
               instance.lx[i] = instance.cost_matrix[i][j]
            end
         end
      end
   
      return instance
   end  
}

